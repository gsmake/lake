#include <vector>
#include <lemon/os/sysinfo.hpp>
#include <lemon/os/exec.hpp>
#include <lemon/fs/fs.hpp>
#include <lemon/strings.hpp>
#include <lemon/os/os_errors.hpp>

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
		const std::string delimiter = ";";
		const std::string extend = ".exe";
#else
		const std::string delimiter = ":";
		const std::string extend = "";
#endif //WIN32

		auto paths = strings::split(std::get<0>(path), delimiter);

#ifdef WIN32
		DWORD length = ::GetSystemDirectoryW(0, 0);

		std::vector<wchar_t> buff(length);

		::GetSystemDirectoryW(&buff[0], buff.size());

		paths.push_back(convert().to_bytes(&buff[0]));
#else
#endif 

		for(auto p : paths)
		{
			auto fullPath = (filepath::path(p) / (cmd + extend)).native();
			
			if(fs::exists(fullPath))
			{
				return std::make_tuple(fullPath, true);
			}
		}

		return std::make_tuple(std::string(), false);
	}


	command::command(const std::string & name)
		:_process(nullptr),_stdin(nullptr),_stdout(nullptr),_stderr(nullptr)
	{
		if(fs::exists(name))
		{
			_path = name;
			return;
		}

		if(fs::exists(name + os::execute_suffix()))
		{
			_path = name + os::execute_suffix();
			return;
		}

		auto path = lookup(name);

		if (!std::get<1>(path)) 
		{
			throw std::system_error((int)os::errc::command_not_found, os::os_error_category(),name);
		}

		_path = std::get<0>(path);
	}

	command::~command()
	{
		if(_process)
		{
			delete _process;
		}
	}


	command & command::setenv(const std::vector<std::string> & env)
	{
		this->_env = env;
		return *this;
	}

	/**
	* set the stdin reader
	*/
	command & command::setstdin(io::reader *reader)
	{
		this->_stdin = reader;
		return *this;
	}
	/**
	* set the stdout writer
	*/
	command & command::setstdout(io::writer * writer)
	{
		this->_stdout = writer;
		return *this;
	}

	/**
	* set the stderr writer
	*/
	command & command::setstderr(io::writer * writer)
	{
		this->_stderr = writer;
		return *this;
	}

	int command::wait()
	{
		return _process->wait();
	}

	command & command::setdir(const std::string & dir)
	{
		_workpath = dir;
		return *this;
	}

}}