//
//  response.cpp
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-11.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#include "response.h"
#include <sstream>

using namespace dns;

int Response::encode(char *buff) {
    char *start = buff;
    encode_header(buff);
    encode_questions(buff);
    encode_answers(buff);
    int len = (int)(buff-start);
    return len;
}

//convert response to string
std::string Response::to_string() {
    std::stringstream stream;
    stream<<"Response:"<<std::endl;
    stream<<"Status:"<<m_rcode<<std::endl;
    for (std::vector<MResource>::iterator iter=m_answers.begin(); iter!=m_answers.end(); iter++) {
        MResource resource = *iter;
        stream<<resource.rIp<<" "<<resource.rType<<std::endl;
    }
    return stream.str();
}