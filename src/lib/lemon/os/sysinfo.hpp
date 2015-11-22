#ifndef LEMON_OS_SYSINFO_HPP
#define LEMON_OS_SYSINFO_HPP
#include <tuple>
#include <string>
#include <lemon/config.h>

namespace lemon{ namespace os{
	/**
	 * get the lemon host name
	 */
	enum class host_t {
		Unknown,Win64,Win32,Linux,Solaris,HPUX,AIX,iOS_Simulator,iOS,OSX,OSX_Unknown
	};

	//
	// get host name
	host_t hostname();

	//
	// get env by name
	std::tuple<std::string,bool> getenv(const std::string&);
}}

#endif //LEMON_OS_SYSINFO_HPP