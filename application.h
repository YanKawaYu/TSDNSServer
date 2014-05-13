//
//  application.h
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-10.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#ifndef __TSDNSServer__application__
#define __TSDNSServer__application__

#include <iostream>
#include "server.h"

namespace dns {
    
class Application {
public:
    Application():m_server() {};
    virtual ~Application() {};
    
    void run();
private:
    Server m_server;
};
    
}

#endif /* defined(__TSDNSServer__application__) */
