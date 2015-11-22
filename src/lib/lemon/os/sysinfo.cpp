#include <lemon/os/sysinfo.hpp>
#include <locale>
#include <vector>

namespace lemon {
	namespace os {

		host_t hostname()
		{

#ifdef WIN32
#ifdef _WIN64
			return host_t::Win64;
#else
			return host_t::Win32;
#endif
#elif defined(__linux)
			return host_t::Linux;
#elif defined(__sun)
			return host_t::Solaris;
#elif defined(__hppa)
			return host_t::HPUX;
#elif defined(_AIX)
			return host_t::AIX;
#elif defined(__APPLE__)
#if TARGET_IPHONE_SIMULATOR
			return host_t::iOS_Simulator;
#elif TARGET_OS_IPHONE
			return host_t::iOS;
#elif TARGET_OS_MAC
			return host_t::OSX;
#else
			return host_t::OSX_Unknown;
#endif
#endif //WIN32
			return host_t::Unknown;
		}

		typedef std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> convert;

#ifdef WIN32
		std::tuple<std::string, bool> getenv(const std::string &name)
		{
			auto namew = convert().from_bytes(name);

			DWORD length = ::GetEnvironmentVariableW(namew.c_str(), NULL, 0);

			if(length == 0)
			{
				return std::make_tuple(std::string(), false);
			}

			std::vector<wchar_t> buff(length);

			::GetEnvironmentVariableW(namew.c_str(), &buff[0], length);

			return std::make_tuple(std::string(buff.begin(), buff.end()), true);
		}
#else 

#endif //WIN32
	}
}