#ifndef __JSON_H__
#define __JSON_H__
#include "ProtocolParser.h"
#include<nlohmann/json.hpp>
#include <vector>
#include <fstream>
using std::ifstream;
using std::vector;
using json = nlohmann::json;
struct strSend{
    int _dataId;
    int _dataLength;
};
class Json
{
public:
    //第一个参数套接字，第二个参数消息类型（1，2，100，200，300），第三个传输的内容
    Json();
    Json(int,int,vector<string>&);
    Json(int,int,const string&);
    ~Json();
private:
    //发送消息的目的端，包括服务器和客户端的套接字。
    int _clientFd;
    int _countsize;
    strSend _str;
    const char* _strS;
};

#endif
