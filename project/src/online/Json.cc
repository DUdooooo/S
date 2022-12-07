#include "../../include/Json.h"
//发送推荐关键字组和网页组时调用这个，传进来一个vector就行
Json::Json(){
    cout<<"Json()"<<endl;
}
Json::Json(int clientFd,int dataId,vector<string> &v)
:_clientFd(clientFd)
,_countsize(0)
{
    cout<<"Json(vector<string>)"<<endl;
    //json 对象，用法网上抄的，什么原理咱也不知道。
    json j;
    int no = 0;
    for(auto &it : v){
        j.push_back(it);
        _countsize+=it.size()+1;
        ++no;
    }
    _countsize+=4*no;
    _str._dataId = dataId;
    _str._dataLength = _countsize;
    cout<<"_countsize:"<<_countsize<<endl;
    string js  = j.dump(); 
    _strS = js.c_str();
    //封装结束，下面时先发送消息类型和长度信息，在发送内容。
    //貌似可以优化以下 吧下面两个要发送的组成一个结构体，这样就不会因为某个
    //信息传输延迟导致后发送的先到达，组成结构体一起发送就没有这个问题。
    //8个字节分别是4个字节消息类型的int型，4个字节消息长度的int型。
    send(_clientFd,&_str,8,0);
    send(_clientFd,_strS,_countsize,0);
}


//发送最终文章内容调用这个，将内容转化为一串string传进来就行。
Json::Json(int clientFd,int dataId,const string &doc)
:_clientFd(clientFd)
{
    cout<<"Json(string)"<<endl;
    json j;
    j.push_back(doc);
    _countsize = doc.size()+4;
    _str._dataId = dataId;
    _str._dataLength = _countsize;
    string js = j.dump();
    _strS = js.c_str();
    send(_clientFd,&_str,8,0);
    send(_clientFd,_strS,_countsize,0);
}
Json::~Json(){
    cout<<"~Json()"<<endl;
};
