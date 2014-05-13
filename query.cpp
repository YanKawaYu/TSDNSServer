//
//  query.cpp
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-11.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#include "query.h"
#include <sstream>

using namespace dns;

void Query::decode(const char *buff, const int size) {
    if (sizeof(MHeader) > size) return;
    decode_header(buff);
    decode_questions(buff);
}

//convert query to string
std::string Query::to_string() {
    std::stringstream stream;
    stream<<"Query:"<<std::endl;
    for (std::vector<MQuestion>::iterator iter=m_questions.begin(); iter!=m_questions.end(); iter++) {
        MQuestion question = *iter;
        stream<<question.qName<<" "<<question.qType<<std::endl;
    }
    return stream.str();
}