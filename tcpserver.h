//
// Created by manu on 19/7/24.
//

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <string>

namespace http {
    class TcpServer {
    public:
        TcpServer(std::string hostIp,int port);
        ~TcpServer();
        void startListen();
        int makeSocket();
        void closeSocket();
        void acceptConnection(int &new_socket);
        std::string buildResponse();
        void sendResponse();
    private:
        std::string ipAddress;
        int m_socket;
        int m_port;
        int new_socket;
        long incomingMessage;
        struct sockaddr_in sockaddress;
        unsigned int socketAddress_len;
        std::string serverMessage;


    };
}

#endif //TCPSERVER_H
