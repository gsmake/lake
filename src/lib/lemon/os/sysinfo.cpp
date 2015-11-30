#include <lemon/os/sysinfo.hpp>
#include <locale>
#include <vector>

#include <stdlib.h>

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif //

namespace lemon { namespace os {
    typedef std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> convert;

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
    #if TARGET_OS_SIMULATOR == 1
        return host_t::iOS_Simulator;
    #elif TARGET_OS_IPHONE == 1
        return host_t::iOS;
    #elif TARGET_OS_MAC == 1
        return host_t::OSX;
    #else
        return host_t::OSX_Unknown;
    #endif
    #endif //WIN32
    }



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

        ::GetEnvironmentVariableW(namew.c_str(), &buff[0], buff.size());

        return std::make_tuple(convert().to_bytes(&buff[0]), true);
    }
    #else

    std::tuple<std::string,bool> getenv(const std::string &name)
    {
        const char *val = ::getenv(name.c_str());

        if(val)
        {
            return std::make_tuple(std::string(val),true);
        }

        return std::make_tuple(std::string(), false);
    }

    #endif //WIN32


    std::string execute_suffix()
    {
    #ifdef WIN32
        return ".exe";
    #else
        return "";
    #endif
    }
}}