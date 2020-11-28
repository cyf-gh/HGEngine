/*
 * @Date: 2020-11-24 11:50:12
 * @LastEditors: cyf
 * @LastEditTime: 2020-11-24 23:31:12
 * @FilePath: \HoneyGame\src\main.cc
 * @Description: What is mind? No matter. What is matter? Nevermind.
 */

#include "EngineImpl/HGEngineImpl.h"

int main( int argc, char **argv ) {
    __HGImpl::V1::HGEngineImpl tEngine( argc, argv );
    tEngine.Run();
    return 0;
}


