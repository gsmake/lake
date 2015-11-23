#include <iostream>
#include <lemon/test/test.hpp>
#include <lemon/os/sysinfo.hpp>
#include <lemon/os/exec.hpp>
#include <lemon/io/readwriter.hpp>

using namespace lemon::os;
using namespace lemon::exec;
test_(getenv) {
	std::cout << getenv("PATH") << std::endl;
}

test_(lookup) {
	auto path = lookup("notepad");

	test_assert(std::get<1>(path));

	std::cout << std::get<0>(path) << std::endl;
}


test_(command) {

	lemon::io::bytes buffIn,buffOut;

	std::cout << "process exit :" << command("netstat").start({"-an"}).wait() << std::endl;
}