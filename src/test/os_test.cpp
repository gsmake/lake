#include <iostream>
#include <lemon/test/test.hpp>
#include <lemon/os/sysinfo.hpp>
#include <lemon/os/exec.hpp>

using namespace lemon::os;
using namespace lemon::exec;
test_(getenv) {
	std::cout << getenv("PATH") << std::endl;
}

test_(lookup) {
	auto path = lookup("notepad.exe");

	test_assert(std::get<1>(path));

	std::cout << std::get<0>(path) << std::endl;
}