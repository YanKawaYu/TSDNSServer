//
//  server.h
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-10.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#ifndef __TSDNSServer__server__
#define __TSDNSServer__server__

#include <iostream>
#include "query.h"
#include "response.h"
#include "resolver.h"

namespace dns {
    
class Server {
public:
    Server();
    virtual ~Server() {};
    
    void init(int&);
    void run();
private:
    int m_socketfd;
    Query m_query;
    Response m_response;
    Resolver m_resolver;
};
    
}



#endif /* defined(__TSDNSServer__server__) */
