#include <complex>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include "tcpserver.h"
#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)


namespace {
    const int BUFFER_SIZE = 30720;
}
namespace http {
    TcpServer::TcpServer(std::string hostIp, int port) : ipAddress(hostIp), m_port(port), m_socket(), new_socket(),
                                                             incomingMessage(),
                                                             sockaddress(), socketAddress_len(sizeof(sockaddress)),
                                                             serverMessage(buildResponse()) {
        std::cout<<"11"<<std::endl;
        sockaddress.sin_family = AF_INET;
        sockaddress.sin_port = htons(m_port);
        sockaddress.sin_addr.s_addr = inet_addr(ipAddress.c_str());
        if(makeSocket() != 0) {
            std::cout<<"cannot create socket from constructor"<<std::endl;
        }



        std::cout<<"12"<<std::endl;
    }
    TcpServer::~TcpServer() {
        closeSocket();
    }
    int TcpServer::makeSocket()
    {
        std::cout<<"4"<<std::endl;
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        std::cout<<m_socket<<std::endl;
        if (m_socket < 0)
        {
            std::cout<<"Cannot create socket"<<std::endl;
            return 1;
            exit(1);

        }
        std::cout<<"14"<<std::endl;

        if (bind(m_socket, (sockaddr *) &sockaddress, sizeof(sockaddress)) < 0)
        {
            std::cout<<bind(m_socket, (sockaddr *) &sockaddress, sizeof(sockaddress))<<std::endl;
            handle_error("bind");
            std::cout<<"Cannot connect socket to address";
            return 1;
            exit(1);
        }
        std::cout<<"5"<<std::endl;
        return 0;
    }

    void TcpServer::closeSocket()
    {
        close(m_socket);
        close(new_socket);
        exit(0);
    }

    void TcpServer::acceptConnection(int &new_socket)
    {
        std::cout<<"7"<<std::endl;
        new_socket = accept(m_socket, (sockaddr *)&sockaddress, &socketAddress_len);
        if (new_socket < 0)
        {
            std::cout<<"Server failed to accept incoming connection from ADDRES";
            exit(1);
        }
    }

    std::string TcpServer::buildResponse()
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }

    void TcpServer::sendResponse()
    {
        long bytesSent;

        bytesSent = write(new_socket, serverMessage.c_str(), serverMessage.size());

        if (bytesSent == serverMessage.size())
        {
            std::cout<<"------ Server Response sent to client ------\n\n";
        }
        else
        {
            std::cout<<"Error sending response to client";
        }
    }

    void TcpServer::startListen()
    {
        std::cout<<"8"<<std::endl;
        if (listen(m_socket, 20) < 0)
        {
            std::cout<<"Socket listen failed";
            exit(1);
        }
        std::cout<<"10"<<std::endl;
        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " << inet_ntoa(sockaddress.sin_addr) << " PORT: " << ntohs(sockaddress.sin_port) << " ***\n\n";
        std::cout<<ss.str();

        int bytesReceived;

        while (true)
        {
            std::cout<<"====== Waiting for a new connection ======\n\n\n";
            acceptConnection(new_socket);

            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = read(new_socket, buffer, BUFFER_SIZE);
            if (bytesReceived < 0)
            {
                std::cout<<"Failed to read bytes from client socket connection";
            }

            std::ostringstream ss;
            ss << "------ Received Request from client ------\n\n";
            std::cout<<ss.str();

            sendResponse();

            close(new_socket);
        }
    }

}
