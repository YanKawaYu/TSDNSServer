//
//  main.cpp
//  TSDNSServer
//
//  Created by zhaoxy on 14-5-10.
//  Copyright (c) 2014年 tsinghua. All rights reserved.
//

#include <iostream>
#include "application.h"

using namespace dns;

int main(int argc, const char * argv[])
{
    Application *application = new Application();
    application->run();
    
    return 0;
}

