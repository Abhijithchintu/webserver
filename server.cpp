
#include <iostream>

#include "tcpserver.h"

int main()
{
    using namespace http;
    std::cout<<"Cannot create socket"<<std::endl;
    TcpServer server = TcpServer("127.0.0.1", 12348);
    server.startListen();

    return 0;
}//
// Created by manu on 19/7/24.
//
