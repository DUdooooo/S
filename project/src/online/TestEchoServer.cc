#include "../../include/EchoServer.h"


int main(int argc, char **argv)
{

    string Ip = GetConfig("IP");
    int port = stoi(GetConfig("PORT").c_str());
    EchoServer es(4, 10, Ip, port);
    es.start();
    return 0;
}

