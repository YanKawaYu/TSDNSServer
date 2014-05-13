//
//  server.cpp
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-10.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>

using namespace dns;

Server::Server() {
    m_resolver.init("/Users/zhaoxy/Desktop/hosts");
}

void Server::init(int &port) {
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    //protocol domain
    servAddr.sin_family = AF_INET;
    //default ip
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //port
    servAddr.sin_port = htons(port);
    //create socket
    if ((m_socketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return;
    }
    unsigned value = 1;
    setsockopt(m_socketfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
    //bind socket to port
    if (bind(m_socketfd, (struct sockaddr *)&servAddr, sizeof(servAddr))) {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return;
    }
    //dynamically allocating a port
    if (port == 0) {
        socklen_t namelen = sizeof(servAddr);
        if (getsockname(m_socketfd, (struct sockaddr *)&servAddr, &namelen) == -1) {
            printf("getsockname error: %s(errno: %d)\n",strerror(errno),errno);
            return;
        }
        port = ntohs(servAddr.sin_port);
    }
    std::cout<<"server running on port:"<<port<<std::endl;
}

void Server::run() {
    char buff[512];
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(struct sockaddr_in);
    while (1) {
        int n = (int)recvfrom(m_socketfd, buff, sizeof(buff), 0, (struct sockaddr *)&clientAddr, &len);
        if (n <= 0) continue;
        m_query.decode(buff, n);
        std::cout<<m_query.to_string();
        
        m_resolver.process(m_query, m_response);
        memset(buff, 0, sizeof(buff));
        n = m_response.encode(buff);
        std::cout<<m_response.to_string();
        sendto(m_socketfd, buff, n, 0, (struct sockaddr *)&clientAddr, len);
        
        std::cout<<std::endl;
    }
}