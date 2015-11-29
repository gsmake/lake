#include <iostream>
#include <lemon/log/log.hpp>
#include <lemon/test/test.hpp>

using namespace lemon::log;

test_(log){
    lemonE(get("test"),"hello world :%s","yayanyang");
	lemonW(get("test"),"hello world :%s","yayanyang");
	lemonI(get("test"),"hello world :%s","yayanyang");
	lemonD(get("test"),"hello world :%s","yayanyang");
	lemonT(get("test"),"hello world :%s","yayanyang");
	lemonV(get("test"),"hello world :%s","yayanyang");

    std::this_thread::sleep_for(std::chrono::seconds(1));
}