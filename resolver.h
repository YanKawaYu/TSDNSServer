//
//  resolver.h
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-11.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#ifndef __TSDNSServer__resolver__
#define __TSDNSServer__resolver__

#include <iostream>
#include "query.h"
#include "response.h"
#include <vector>

namespace dns {

class Resolver {
public:
    Resolver() {};
    virtual ~Resolver() {};
    
    void init(const std::string&);
    void process(const Query&, Response&);
protected:
    struct Host{
        std::string ipAddr;
        std::string name;
    };
    
    std::vector<Host> m_hosts;
};
    
}

#endif /* defined(__TSDNSServer__resolver__) */
