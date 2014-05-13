//
//  application.cpp
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-10.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#include "application.h"

using namespace dns;

void Application::run() {
    int port = 53;
    m_server.init(port);
    m_server.run();
}