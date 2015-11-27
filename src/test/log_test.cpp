#include <iostream>
#include <lemon/log/log.hpp>
#include <lemon/test/test.hpp>

using namespace lemon::log;

test_(log){
    logE(get("test"),"hello world :%s","yayanyang");
    logW(get("test"),"hello world :%s","yayanyang");
    logI(get("test"),"hello world :%s","yayanyang");
    logD(get("test"),"hello world :%s","yayanyang");
    logT(get("test"),"hello world :%s","yayanyang");
    logV(get("test"),"hello world :%s","yayanyang");

    std::this_thread::sleep_for(std::chrono::seconds(1));
}