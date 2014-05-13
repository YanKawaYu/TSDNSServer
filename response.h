//
//  response.h
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-11.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#ifndef __TSDNSServer__response__
#define __TSDNSServer__response__

#include <iostream>
#include "message.h"
#include "query.h"

namespace dns {

class Response : public Message {
public:
    Response() {};
    virtual ~Response() {};
    
    int encode(char *);
    std::string to_string();
};
    
}

#endif /* defined(__TSDNSServer__response__) */
