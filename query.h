//
//  query.h
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-11.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#ifndef __TSDNSServer__query__
#define __TSDNSServer__query__

#include <iostream>
#include "message.h"

namespace dns {

class Query : public Message {
public:
    Query() {};
    virtual ~Query() {};
    
    void decode(const char*, const int);
    std::string to_string();
};
    
}

#endif /* defined(__TSDNSServer__query__) */
