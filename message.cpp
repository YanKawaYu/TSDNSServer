//
//  message.cpp
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-11.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#include "message.h"

using namespace dns;
using namespace std;

//encode an address seperated by '.' like 'www.google.com'
//to address seperated by substring length like '3www6google3com'
void encode_address(char *addr, char *&buff) {
    string address(addr);
    int pos, current = 0;
    while ((pos = (int)address.find('.')) != string::npos) {
        address.erase(0, pos+1);
        *buff++ = pos;
        memcpy(buff, addr+current, pos);
        buff += pos;
        current += pos+1;
    }
    *buff++ = address.size();
    memcpy(buff, addr+current, address.size());
    buff += address.size();
    
    *buff++ = 0;
}

//encode the message to the buffer
void Message::encode_header(char *&buff) {
    MHeader header = {0};
    header.hId = m_id;
    header.hFlags += ((m_qr?1:0)<<15);
    header.hFlags += (m_opcode<<11);
    header.hFlags += (m_aa<<10);
    header.hFlags += (m_tc<<9);
    header.hFlags += (m_rd<<8);
    header.hFlags += (m_ra<<7);
    header.hFlags += m_rcode;
    header.queryCount = m_qdCount;
    header.answCount = m_anCount;
    header.authCount = m_nsCount;
    header.addiCount = m_arCount;
    
    header.hId = htons(header.hId);
    header.hFlags = htons(header.hFlags);
    header.queryCount = htons(header.queryCount);
    header.answCount = htons(header.answCount);
    header.authCount = htons(header.authCount);
    header.addiCount = htons(header.addiCount);
    
    memcpy(buff, &header, sizeof(MHeader));
    //offset
    buff += sizeof(MHeader);
}

//encode the questions of message to the buffer
void Message::encode_questions(char *&buff) {
    //encode each question
    for (int i=0; i<m_qdCount; i++) {
        MQuestion question = m_questions[i];
        encode_address(question.qName, buff);
        
        uint16_t nQType = htons(question.qType);
        memcpy(buff, &nQType, sizeof(uint16_t));
        buff+=sizeof(uint16_t);
        
        uint16_t nQClass = htons(question.qClass);
        memcpy(buff, &nQClass, sizeof(uint16_t));
        buff+=sizeof(uint16_t);
    }
}

//encode the answers of the message to the buffer
void Message::encode_answers(char *&buff) {
    //encode each answer
    for (int i=0; i<m_anCount; i++) {
        MResource resource = m_answers[i];
        encode_address(resource.rName, buff);
        
        uint16_t nRType = htons(resource.rType);
        memcpy(buff, &nRType, sizeof(uint16_t));
        buff+=sizeof(uint16_t);
        
        uint16_t nRClass = htons(resource.rClass);
        memcpy(buff, &nRClass, sizeof(uint16_t));
        buff+=sizeof(uint16_t);
        
        uint32_t nTTL = htonl(resource.rTTL);
        memcpy(buff, &nTTL, sizeof(uint32_t));
        buff+=sizeof(uint32_t);
        
        uint16_t nRDLen = htons(resource.rdLength);
        memcpy(buff, &nRDLen, sizeof(uint16_t));
        buff+=sizeof(uint16_t);
        
        if (MT_A == resource.rType) {
            memcpy(buff, resource.rData, sizeof(uint32_t));
            buff+=sizeof(uint32_t);
        }
    }
}

//decode the message header from the buffer
void Message::decode_header(const char *&buff) {
    MHeader header;
    memcpy(&header, buff, sizeof(MHeader));
    //network order to host order
    header.hId = ntohs(header.hId);
    header.hFlags = ntohs(header.hFlags);
    header.queryCount = ntohs(header.queryCount);
    header.answCount = ntohs(header.answCount);
    header.authCount = ntohs(header.authCount);
    header.addiCount = ntohs(header.addiCount);
    //id
    m_id = header.hId;
    //flags
    m_qr = header.hFlags&QR_MASK;
    m_opcode = header.hFlags&OPCODE_MASK;
    m_aa = header.hFlags&AA_MASK;
    m_tc = header.hFlags&TC_MASK;
    m_rd = header.hFlags&RD_MASK;
    m_ra = header.hFlags&RA_MASK;
    m_rcode = header.hFlags&RCODE_MASK;
    //count
    m_qdCount = header.queryCount;
    m_anCount = header.answCount;
    m_nsCount = header.authCount;
    m_arCount = header.addiCount;
    //offset
    buff+= sizeof(MHeader);
}

//decode the questions of the message from the buffer
void Message::decode_questions(const char *&buff) {
    //reset
    m_questions.clear();
    //decode each question
    for (int i=0; i<m_qdCount; i++) {
        MQuestion question = {0};
        //name
        while (1) {
            uint len = *buff++;
            if (len==0) break;
            if (strlen(question.qName)!=0) strcat(question.qName, ".");
            memcpy(question.qName+strlen(question.qName), buff, len);
            buff+=len;
        }
        //type
        question.qType = ntohs(*((uint16_t *)buff));
        buff+=sizeof(uint16_t);
        //class
        question.qClass = ntohs(*((uint16_t *)buff));
        buff+=sizeof(uint16_t);
        //add to list
        m_questions.push_back(question);
    }
}