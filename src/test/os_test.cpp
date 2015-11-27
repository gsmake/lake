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

	lemon::io::bytes buffIn,buffOut;

    auto exitCode = command("ls").setstdin(&buffIn).setstdout(&buffOut).start({}).wait();

    std::cout << "process exit :" << exitCode << std::endl;

    std::cout << "buffer size :" << buffIn.buff().size() << std::endl;
}