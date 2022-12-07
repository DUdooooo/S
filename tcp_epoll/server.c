#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

#define MAXEVENTS 2048
#define BUFFSIZE 1000
#define CHANNELSIZE 100


//业务层 json对象

enum MessageType {
	MESSAGE_REGINSTER,
	MESSAGE_LOGIN,
	MESSAGE_COMMAND,
};

//协议层
typedef struct Train {
	int length;
	MessageType messageType;
	char buff[1000];
}train_t;


//tcp通道相关
typedef struct TcpChannel {
	int netfd;
	int recvLength;
	int sendLength;
	char recvbuff[BUFFSIZE];//应用层接收缓冲区
	char sendbuff[BUFFSIZE];//应用层发送缓冲区
} tcpchannel_t;

int channelSize = 0;//记录当前有多少通道已建立好连接了
tcpchannel_t channels[CHANNELSIZE] = {0};//维护已经建立好连接的通道信息

//通道管理操作
void initChannels();
int currentChannelIndex(int netfd);
int insertChannel(int netfd);
void deleteChannel(int netfd);

//对某一个通道的操作
void clearSendBuffer(int idx);
void clearRecvBuffer(int idx);

//tcp相关
int tcpInit(const char * ip, unsigned short port);
struct sockaddr_in tcpLocalInetAddress(int netfd);
struct sockaddr_in tcpPeerInetAddress(int netfd);
char * getIP(struct sockaddr_in addr);
unsigned short getPort(struct sockaddr_in addr);


//epoll相关
int epollInit();
int epollAddReadEvent(int epfd, int fd);
int epollAddReadWriteEvent(int epfd, int fd);
int epollSetWriteEvent(int epfd, int fd);
int epollClearWriteEvent(int epfd, int fd);
int epollDelEvent(int epfd, int fd);

void epollEventLoop(int epfd, int listenfd);

typedef void(*TcpCallback)(int);
void epollHandleNewConnection(int epfd, int listenfd, TcpCallback connectCb);
void epollHandleReadEvent(int epfd, int netfd, TcpCallback messageCb, TcpCallback closeCb);
void epollHandleWriteEvent(int epfd, int netfd);

//连接建立时的回调函数
void connectCallback(int netfd)
{
	char buff[100] = {0};
	sprintf(buff, ">> tcp conn [%s:%d --> %s:%d] has connected successfully!",
			getIP(tcpLocalInetAddress(netfd)),
			getPort(tcpLocalInetAddress(netfd)),
			getIP(tcpPeerInetAddress(netfd)),
			getPort(tcpPeerInetAddress(netfd)));
	printf("%s\n", buff);
}

//连接断开时的处理
void closeCallback(int netfd) {
	char buff[100] = {0};
	sprintf(buff, ">> tcp conn [%s:%d --> %s:%d] has already closed!",
			getIP(tcpLocalInetAddress(netfd)),
			getPort(tcpLocalInetAddress(netfd)),
			getIP(tcpPeerInetAddress(netfd)),
			getPort(tcpPeerInetAddress(netfd)));
	printf("%s\n", buff);
}

//消息达到时的处理(编解码)
void messageCallback(int netfd) 
{
	//decode, compute, encode
	int i = currentChannelIndex(netfd);
	if(i != -1) {
		//执行回显服务: 在应用层将接收缓存区中的数据copy到发送缓冲区
		strncpy(channels[i].sendbuff,
				channels[i].recvbuff,
				channels[i].recvLength);
		channels[i].sendLength = channels[i].recvLength;
	}
}

int main(int argc, const char *argv[])
{
	initChannels();
	int listenfd = tcpInit("127.0.0.1", 8888);
	int epfd = epollInit();

	epollEventLoop(epfd, listenfd);

	close(epfd);
    close(listenfd);
    return 0;
}

int tcpInit(const char * ip, unsigned short port)
{
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    if(listenfd == -1)
        ERR_EXIT("socket");

    //地址复用
    int on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        ERR_EXIT("setsockopt");
	}

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8888);

    if(bind(listenfd, (struct sockaddr*)&addr, sizeof addr) == -1) {
        ERR_EXIT("bind");
	}

    if(listen(listenfd, SOMAXCONN) == -1) {
        ERR_EXIT("listen");
	}
	return listenfd;
}

int epollInit()
{
	int efd = epoll_create1(0);//推荐使用这种方式
	if(-1 == efd)
	{
		perror("epoll_create error");
		exit(EXIT_FAILURE);
	}
	
	return efd;
}

int epollAddReadEvent(int epfd, int fd)
{
	struct epoll_event evt;
	evt.data.fd = fd;
	evt.events = EPOLLIN;//| EPOLLET//(边缘触发);
	//通过epoll_ctl操作的是内核中的那一颗红黑树
	//第三个参数相当于一个关键字
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evt);
	if(-1 == ret)
	{
		perror("epoll_ctl");
		return -1;
	}
	return 0;
}

int epollAddReadWriteEvent(int epfd, int fd)
{
	struct epoll_event evt;
	evt.data.fd = fd;
	evt.events = EPOLLIN | EPOLLOUT;
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evt);
	if(-1 == ret)
	{
		perror("epoll_ctl");
		return -1;
	}
	return 0;
}

int epollSetWriteEvent(int epfd, int fd)
{
	struct epoll_event evt;
	evt.data.fd = fd;
	evt.events = EPOLLIN | EPOLLOUT;//修改之后，监听读写事件
	int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &evt);
	if(-1 == ret) {
		perror("epoll_ctl");
		return -1;
	}
	return 0;
}

int epollClearWriteEvent(int epfd, int fd)
{
	struct epoll_event evt;
	evt.data.fd = fd;
	evt.events = EPOLLIN;//修改之后，只监听读事件,不再监听写事件
	int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &evt);
	if(-1 == ret) {
		perror("epoll_ctl");
		return -1;
	}
	return 0;
}

int epollDelEvent(int epfd, int fd)
{
	struct epoll_event evt;
	evt.data.fd = fd;
	int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &evt);
	if(-1 == ret) {
		perror("epoll_ctl");
		return -1;
	}
	return 0;
}

void epollEventLoop(int epfd, int listenfd) {

	epollAddReadEvent(epfd, listenfd);
	struct epoll_event *events = calloc(MAXEVENTS, sizeof(struct epoll_event));
	//事件循环
	while(1)
	{
		int nready = epoll_wait(epfd, events, MAXEVENTS, 5000);	
		if(nready == 0) {
			printf("\n epoll_wait timeout!\n");
		} else if(nready == -1 && errno == EINTR) {
			continue;
		} else if(nready == -1){
			perror("epoll_wait"); return ;
		} else { 
			for(int idx = 0; idx != nready; ++idx) {
				int netfd = events[idx].data.fd;
				if(netfd == listenfd){	//新客户端连接到来
					epollHandleNewConnection(epfd, listenfd, connectCallback);
				} else {//与客户端建立的fd --> 已经建立的链接(旧链接)
					//处理读事件
					if(events[idx].events & EPOLLIN){
						epollHandleReadEvent(epfd, netfd, messageCallback, closeCallback);
					}
					//处理写事件
					if(events[idx].events & EPOLLOUT) {
						epollHandleWriteEvent(epfd, netfd);
					}
				}
			}
		}
	}
}

void epollHandleNewConnection(int epfd, int listenfd, TcpCallback connectCb)
{
	int netfd = accept(listenfd, NULL, NULL);
	if(-1 == netfd) {
		perror("accept");
		return ;
	}

	epollAddReadEvent(epfd, netfd);
	insertChannel(netfd);

	//建立新连接之后，要打印一些信息
	//考虑扩展性，直接写死在这里，就是不合理的
	//printf("client:  ->server: has connected!\n"); 
	//printf("serverip:port  ->clientip:port has connected!\n"); 
    //事件处理器用函数对象(回调函数)进行抽象
	connectCb(netfd);
}

void epollHandleReadEvent(int epfd, int netfd, TcpCallback messageCb, TcpCallback closeCb) {
	int i = currentChannelIndex(netfd);
	if(-1 == i) {
		return;
	}

	char * pbuf = channels[i].recvbuff;
	int recvLength = channels[i].recvLength;

	//1. 先判断连接是否断开
	int nread = 0;
	do{
		nread = recv(netfd, pbuf + recvLength, BUFFSIZE - recvLength, 0);
	} while(nread == -1 && errno == EINTR);

	if(0 == nread) { //2. 链接断开之后的处理
		epollDelEvent(epfd, netfd);//从epoll监听队列中删除
		closeCb(netfd);//定制化当链接断开的时候,要做的处理方式
		deleteChannel(netfd);//从Tcp通道的管理中删除
		return;
	} else if(nread == -1) {
		perror("recv");
		return;
	} else { 
		//更新接收缓冲区已接收的字节数
		channels[i].recvLength += nread;

		//2. 对消息进行处理(回调函数,定制化处理)
		messageCb(netfd);

		//3. 如果netfd的发送缓冲区中有数据,
		//将netfd设置为可写, 但并不执行真正的发送操作
		//当下一次epoll_wait返回时，才执行发送操作
		if(channels[i].sendLength > 0) {
			epollSetWriteEvent(epfd, netfd);
		}
		//4. 清空接收缓存区中
		clearRecvBuffer(i);
	}
}

void epollHandleWriteEvent(int epfd, int netfd)
{
	//printf("\nbegin send...\n");
	int i = currentChannelIndex(netfd);
	if(i != -1) {
		char * pbuf = channels[i].sendbuff;
		int sendLength = channels[i].sendLength;
		int nsend = send(netfd, pbuf, sendLength, 0);	
		if(nsend == sendLength) {
			//1.如果发送缓冲区中的所有数据全部发送完成
			//后续不再监听写事件
			clearSendBuffer(i);
			epollClearWriteEvent(epfd, netfd);
		} else if(nsend > 0 && nsend < sendLength) {
			//2. 如果只发送了部分数据, 将未发送的数据移到开始位置
			channels[i].sendLength = sendLength - nsend;
			memmove(channels[i].sendbuff, channels[i].sendbuff + nsend, sendLength - nsend);
		}
		printf("\nsend over...\n");
	}
}

struct sockaddr_in tcpLocalInetAddress(int netfd)
{
	struct sockaddr_in localAddr;
	socklen_t len = sizeof(struct sockaddr_in);
	if(getsockname(netfd, (struct sockaddr*)&localAddr, &len) < 0) {
		perror("getsockname");
	}
	return localAddr;
}

struct sockaddr_in tcpPeerInetAddress(int netfd)
{
	struct sockaddr_in peerAddr;
	socklen_t len = sizeof(struct sockaddr_in);
	if(getpeername(netfd, (struct sockaddr*)&peerAddr, &len) < 0) {
		perror("getpeername");
	}
	return peerAddr;
}
char *getIP(struct sockaddr_in addr)
{
	return inet_ntoa(addr.sin_addr);
}

unsigned short getPort(struct sockaddr_in addr)
{
	return ntohs(addr.sin_port);
}

void initChannels() {
	for(int i = 0; i < CHANNELSIZE; ++i) {
		channels[i].netfd = -1;
		channels[i].recvLength = 0;
		channels[i].sendLength = 0;
		memset(channels[i].recvbuff, 0, BUFFSIZE);
		memset(channels[i].sendbuff, 0, BUFFSIZE);
	}
}

int currentChannelIndex(int netfd) {
	for(int i = 0; i < CHANNELSIZE; ++i) {
		if(channels[i].netfd == netfd) {
			return i;//找到该通道所在的下标
		}
	}
	return -1;//不在通道中
}

int insertChannel(int netfd) {
	for(int i = 0; i < CHANNELSIZE; ++i) {
		if(-1 == channels[i].netfd) {
			channels[i].netfd = netfd;
			++channelSize;
			return i;
		}
	}
	return -1;
}

void deleteChannel(int netfd)
{
	int i = currentChannelIndex(netfd);
	if(i != -1) {
		close(netfd);
		channels[i].netfd = -1;
		memset(channels[i].recvbuff, 0, BUFFSIZE);
		memset(channels[i].sendbuff, 0, BUFFSIZE);
		channels[i].recvLength = 0;
		channels[i].sendLength = 0;
		--channelSize;
	}
}

void clearSendBuffer(int idx) {
	memset(channels[idx].sendbuff, 0, BUFFSIZE);
	channels[idx].sendLength = 0;
}

void clearRecvBuffer(int idx) {
	memset(channels[idx].recvbuff, 0, BUFFSIZE);
	channels[idx].recvLength = 0;
}
