#include "../../include/Json.h"
using std::cin;
#define MAXEVENTS 10
typedef struct fd1{
    int clientfd;
    int fd;    //用户需要下载的文件（通过open在本地创建）的 文件描述符
}client_t;
typedef struct tra{
    int _dataId;
    int _dataLength;
    char _data[1000000];
}_train;



int main()
{
    int clientfd = socket(AF_INET,SOCK_STREAM,0);
    client_t c;
    c.clientfd=clientfd;
    struct sockaddr_in clientaddr;
    clientaddr.sin_family=AF_INET;
    clientaddr.sin_port=htons(8888);
    clientaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    connect(clientfd,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
    int epfd = epoll_create(MAXEVENTS);
    struct epoll_event peventlist[MAXEVENTS]; 
    memset(&peventlist,0,sizeof(peventlist));
    struct epoll_event evt;

    evt.data.fd=EPOLLIN;
    evt.events=EPOLLIN|EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,EPOLLIN,&evt);
    pthread_t pid;
    while(1){
        int ret = epoll_wait(epfd,peventlist,MAXEVENTS,1000);
        if(ret==0){
        }
        if(ret>0){
            for(int idx = 0;idx<ret;idx++){
                while(1){
                    _train _train;
                    _train._dataId = 1;
                    while(!(_train._dataId-1)){
                        memset(&_train,0,sizeof(_train));
                        cin>>_train._dataId;
                        read(STDIN_FILENO,_train._data,sizeof(_train._data));
                        _train._dataLength = strlen(_train._data);
                        send(clientfd,&_train,8+_train._dataLength,0);
                        if(_train._dataId==2){
                            break ;
                        }
                        ProtocolParser getRecWord(clientfd);
                    }
                    int count = 0;
                    recv(clientfd,&count,4,MSG_WAITALL);
                    if(count == 0){
                        cout<<"该关键词没有文章，请从新输入"<<endl;
                        int num = 0;
                        cout <<">>please select number:" << endl;
                        cout << "1:Query again" << endl;
                        cout << "2:exit" << endl;
                        cin>>num;
                        if(2==num)
                        {
                            exit(1);
                        }
                        continue;
                    }
                    cout<<"count"<<count<<endl;
                    int no = 1;
                    for(int o = 0;o<count;++o){
                    cout<<"第 "<<no++<<" 篇文章"<<endl;
                    ProtocolParser getRecPage(clientfd);
                    /* ProtocolParser ~ProtocolParser(); */
                    }
                    /* cout<<"213213"<<endl; */
                    /* cout<<"第 "<<no++<<" 篇文章"<<endl; */

                    /* ProtocolParser getRecPage2(clientfd); */
                    /* cout<<"第 "<<no++<<" 篇文章"<<endl; */
                    /* ProtocolParser getRecPage3(clientfd); */
                    /* cout<<"第 "<<no++<<" 篇文章"<<endl; */
                    /* ProtocolParser getRecPage4(clientfd); */
                    /* cout<<"第 "<<no++<<" 篇文章"<<endl; */
                    /* ProtocolParser getRecPage5(clientfd); */
                    /* cout<<"第 "<<no++<<" 篇文章"<<endl; */
                    /* ProtocolParser getRecPage6(clientfd); */
                    /* cout<<"第 "<<no++<<" 篇文章"<<endl; */
                    /* ProtocolParser getRecPage7(clientfd); */
                    /* cout<<"第 "<<no++<<" 篇文章"<<endl; */
                    /* ProtocolParser getRecPage8(clientfd); */
                    /* cout<<"第 "<<no++<<" 篇文章"<<endl; */
                    /* ProtocolParser getRecPage9(clientfd); */
                    /* cout<<"第 "<<no++<<" 篇文章"<<endl; */
                    /* ProtocolParser getRecPage10(clientfd); */
                    cout<<">>>>>please choice the page which you want get"<<endl;
                    /* cout<<"saczxc"<<endl; */
                    int choiceNUm = 0;
                    cin>>choiceNUm;
                    send(clientfd,&choiceNUm,4,0);
                    ProtocolParser getFinalPage(clientfd);
                    /* exit(1); */
                    int num = 0;
                    cout <<">>please select number:" << endl;
                    cout << "1:Query again" << endl;
                    cout << "2:exit" << endl;
                    cin>>num;
                    if(2==num)
                    {
                        exit(1);
                    }
                }
            }
        }

    }
    pthread_join(pid,NULL);
    return 0;
}

