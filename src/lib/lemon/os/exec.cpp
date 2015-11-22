#include <vector>
#include <lemon/os/sysinfo.hpp>
#include <lemon/os/exec.hpp>
#include <lemon/fs/fs.hpp>
#include <lemon/strings.hpp>

namespace lemon {namespace exec {

	typedef std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> convert;

	std::tuple<std::string, bool> lookup(const std::string & cmd)
	{
		auto path = os::getenv("PATH");

		if(!std::get<1>(path))
		{
			return std::make_tuple(std::string(),false);
		}
		
#ifdef WIN32
		const std::string seperator = ";";
#else
		const std::string seperator = ":";
#endif //WIN32

		auto paths = strings::split(std::get<0>(path), seperator);

#ifdef WIN32
		DWORD length = ::GetSystemDirectoryW(0, 0);

		std::vector<wchar_t> buff(length);

		::GetSystemDirectoryW(&buff[0], buff.size());

		paths.push_back(convert().to_bytes(std::wstring(buff.begin(), buff.end())));
#else
#endif 

		for(auto p : paths)
		{
			auto fullpath = (filepath::path(p) / cmd).native();
			
			if(fs::exists(fullpath))
			{
				return std::make_tuple(fullpath, true);
			}
		}

		return std::make_tuple(std::string(), false);
	}


	command::command(const std::string & name)
	{

	}

	command & command::setenv(const std::vector<std::string> & env)
	{
		return *this;
	}

	void command::start(std::initializer_list<std::string> args)
	{

	}

	/*
	* wait command return
	*/
	int command::wait()
	{
		return 0;
	}

	/**
	* set the stdin reader
	*/
	command & command::setstdin(io::reader *reader)
	{
		return *this;
	}
	/**
	* set the stdout writer
	*/
	command & command::setstdout(io::writer * writer)
	{
		return *this;
	}
}}