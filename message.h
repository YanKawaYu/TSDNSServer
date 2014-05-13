//
//  message.h
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-11.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#ifndef __TSDNSServer__message__
#define __TSDNSServer__message__

#include <iostream>
#include <vector>

namespace dns {
    
class Message {
    friend class Resolver;
private:
    static const uint16_t QR_MASK = 0x8000;
    static const uint16_t OPCODE_MASK = 0x7800;
    static const uint16_t AA_MASK = 0x0400;
    static const uint16_t TC_MASK = 0x0200;
    static const uint16_t RD_MASK = 0x0100;
    static const uint16_t RA_MASK = 0x8000;
    static const uint16_t RCODE_MASK = 0x000F;
    
    enum MQueryType //the resource info which query
    {
        MT_A=0x01, //ipv4 address
        MT_NS=0x02, //dns name server
        MT_CNAME=0x05, //host name
        MT_SOA=0x06, //the first auth server
        MT_PTR=0x0C, //
        MT_HINFO=0x0D, //host info
        MT_MINFO=0x0E, //main info
        MT_MX=0x0F, //mail exchange
        MT_TXT=0x10, //text
        MT_UINFO=0x64, //user info
        MT_UID=0x65, //user id
        MT_GID=0x66, //group id
        MT_ANY=0xFF //any type of data
    };
    
    enum MResponseCode //the status code server response
    {
        MC_NO_ERROR=0, //no error
        MC_FORMAT_ERROR=1, //format error
        MC_SERVER_ERROR=2, //server error
        MC_NAME_ERROR=3, //name error
        MC_NOT_SUPPORT=4, //server not support
        MC_REFUSE=5 //server refuse
    };
protected:
    struct MHeader {
        uint16_t hId;
        uint16_t hFlags;
        uint16_t queryCount;
        uint16_t answCount;
        uint16_t authCount;
        uint16_t addiCount;
    };
    
    struct MQuestion {
        //assume that the max length of name is 64
        char qName[64];
        uint16_t qType;
        uint16_t qClass;
    };
    
    struct MResource {
        //assume that the max length of name is 64
        char rName[64];
        uint16_t rType;
        uint16_t rClass;
        uint32_t rTTL;
        uint16_t rdLength;
        char rData[64];
        char rIp[64];
    };

    uint m_id;
    bool m_qr;
    uint m_opcode;
    uint m_aa;
    uint m_tc;
    uint m_rd;
    uint m_ra;
    uint m_rcode;
    
    uint m_qdCount;
    uint m_anCount;
    uint m_nsCount;
    uint m_arCount;
    
    std::vector<MQuestion> m_questions;
    std::vector<MResource> m_answers;
    
    void decode_header(const char*&);
    void decode_questions(const char*&);
    void encode_header(char *&);
    void encode_questions(char *&);
    void encode_answers(char *&);
public:
    Message() {};
    virtual ~Message() {};
    
    uint getID() const { return m_id; };
    uint getOpcode() const { return m_opcode; }
    uint getQdCount() const { return m_qdCount; }
    uint getAnCount() const { return m_anCount; }
    uint getNsCount() const { return m_nsCount; }
    uint getArCount() const { return m_arCount; }
    std::vector<MQuestion> getQuestions() const { return m_questions; };
};
    
}

#endif /* defined(__TSDNSServer__message__) */
