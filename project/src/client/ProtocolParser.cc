#include "../../include/Json.h" 
ProtocolParser::ProtocolParser(const int clientfd)
:_clientFd(clientfd)
{
    process();
};

ProtocolParser::~ProtocolParser(){};

void ProtocolParser::process(){
    memset(_data,0,sizeof(_data));
    //接受消息类型
    int ret = recv(_clientFd,&_dataId,4,MSG_WAITALL);
    if(ret == 0){
        //客户端连接关闭
        cout<<_clientFd<<" is close"<<endl;
        close(_clientFd);
    }
    //接受消息长度信息。
    recv(_clientFd,&_dataLength,4,MSG_WAITALL);
    if(_dataId==1){
        //上面两个是客户端使用的功能。发送数据
        recv(_clientFd,&_data[0],_dataLength,MSG_WAITALL);
    }else if(_dataId==2){
        recv(_clientFd,&_data[0],_dataLength,MSG_WAITALL);
    }else if(_dataId==100){
        //和下面一样,下面三个都是服务器调用的
        char t[1000000] = {0};
        memset(t,0,sizeof(t));
        recv(_clientFd,&t[0],_dataLength,MSG_WAITALL);
        json x = json::parse(t);
        for(auto &it:x){
            cout<<it<<endl;
        }
        cout<<">>>>>please choice message type and the word which you want get"<<endl;

    }else if(_dataId==200){
        //接受对端传输内容
        cout<<_dataId<<endl;
        cout<<_dataLength<<endl;
        char t[1000000];
        memset(t,0,sizeof(t));
        recv(_clientFd,&t[0],_dataLength,MSG_WAITALL);
        //json格式转换
        json x = json::parse(t);
        //输出
        /* int no = 0; */
        /* int three = 0; */
        for(auto &it:x){
            /* if(three%3 == 0){ */
                /* cout<<"第 "<<no++<<" 篇文章"<<endl; */
            /* } */
            cout<<it<<endl;
            /* three++; */
        }
        /* cout<<"please choice the page which you want to get"<<endl; */
    }else if(_dataId == 300){
        //和上面一样，因为是最后一步，所以只少了最后一句提示客户端输出
        char t[1000000];
        memset(t,0,sizeof(t));
        recv(_clientFd,&t,_dataLength,MSG_WAITALL);
        json x = json::parse(t);
        for(auto &it:x){
            cout<<it<<endl;
        }
    }
}

int ProtocolParser::getDataid(){
    return _dataId;
}

string ProtocolParser::getData(){
    return _data;
}
