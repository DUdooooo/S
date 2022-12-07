#ifndef __PROTOCOLPARSER_H__
#define  _PROTOCPLPARSER_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <shadow.h>
#include <unistd.h>
#include <crypt.h>

#include <string>
#include <iostream>
using std::endl;
using std::cout;
using std::string;
typedef struct Train{
    int _dataId;
    int _dataLength;
    //进程之间传输不能用string  所有这里用了char
    char _data[1000000];
}Train;

class ProtocolParser
{
public:
    ProtocolParser(const int);
    ~ProtocolParser();
    void process();
    int getDataid();
    string getData();
private:
    int _clientFd;
    int _dataId;
    int _dataLength;
    char _data[1000000];

};

#endif
