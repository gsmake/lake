#include <iostream>
#include <lemon/test/test.hpp>
#include <lemon/os/sysinfo.hpp>
#include <lemon/os/exec.hpp>
#include <lemon/io/readwriter.hpp>

using namespace lemon::os;
using namespace lemon::exec;
test_(getenv) {

    auto path = lemon::os::getenv("GSMAKE_HOME");

    test_assert(std::get<1>(path));

    std::cout << std::get<0>(path) << std::endl;
}

test_(lookup) {
#ifdef WIN32
	auto path = lookup("notepad");
#else
    auto path = lookup("ls");
#endif

	test_assert(std::get<1>(path));

	std::cout << std::get<0>(path) << std::endl;

}


test_(command) {

	/*lemon::io::pipe buffIn(1024),buffOut(1024);

    auto exitCode = command("git").setstdin(&buffIn).setstdout(&buffOut).start({"clone","a"}).wait();

    std::cout << "process exit :" << exitCode << std::endl;*/
}