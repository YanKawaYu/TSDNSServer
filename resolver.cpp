//
//  resolver.cpp
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-11.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#include "resolver.h"
#include <fstream>
#include <sstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace dns;
using namespace std;

void Resolver::init(const std::string &fileName) {
    ifstream fstream(fileName);
    char hostStr[128];
    while (fstream.getline(hostStr, sizeof(hostStr))) {
        stringstream sstream;
        sstream<<hostStr;
        Host host;
        sstream>>host.ipAddr;
        sstream>>host.name;
        m_hosts.push_back(host);
    }
}

void Resolver::process(const Query &query, Response &response) {
    //clear
    response.m_questions.clear();
    response.m_answers.clear();
    //find host and generate answers
    vector<Response::MQuestion> questions = query.getQuestions();
    for (vector<Response::MQuestion>::iterator qIter = questions.begin(); qIter != questions.end(); ++qIter) {
        Response::MQuestion question = *qIter;
        Response::MResource resource;
        for (vector<Host>::iterator hIter = m_hosts.begin(); hIter != m_hosts.end(); hIter++) {
            Host host = *hIter;
            //if find
            if (question.qName == host.name && question.qType == Message::MT_A) {
                strcpy(resource.rName, question.qName);
                resource.rType = question.qType;
                resource.rClass = question.qClass;
                resource.rTTL = 10;
                resource.rdLength = sizeof(uint32_t);
                memcpy(resource.rIp, host.ipAddr.c_str(), host.ipAddr.size());
                
                struct sockaddr_in adr_inet;
                memset(&adr_inet, 0, sizeof(adr_inet));
                inet_aton(host.ipAddr.c_str(), &adr_inet.sin_addr);
                memcpy(resource.rData, &adr_inet.sin_addr.s_addr, sizeof(uint32_t));
                
                response.m_answers.push_back(resource);
                
                break;
            }
        }
        
        response.m_questions.push_back(question);
    }
    
    response.m_id = query.getID();
    response.m_qr = 1;
    response.m_opcode = query.getOpcode();
    response.m_aa = 0;
    response.m_tc = 0;
    response.m_rd = 0;
    response.m_ra = 0;
    if (response.m_answers.size()!=response.m_questions.size()) {
        response.m_rcode = Message::MC_SERVER_ERROR;
    }else {
        response.m_rcode = Message::MC_NO_ERROR;
    }
    
    response.m_qdCount = (int)response.m_questions.size();
    response.m_anCount = (int)response.m_answers.size();
    response.m_nsCount = 0;
    response.m_arCount = 0;
}