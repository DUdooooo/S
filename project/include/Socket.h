#ifndef __SOCKET_H__
#define __SOCKET_H__
#include"Redis.h"
#include"Json.h"
#include"KeyRecommander.h"
#include"WebPageQuery.h"
#include "NonCopyable.h"

class Socket
: NonCopyable
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int fd() const;
    void shutDownWrite();

private:
    int _fd;
};

#endif
