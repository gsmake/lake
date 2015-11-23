#include <lemon/os/exec.hpp>

#ifdef WIN32

#include <locale>
#include <sstream>

namespace lemon{namespace exec{
	typedef std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>, wchar_t> convert;

	class win32_process : public os::process
	{
	public:
		
		win32_process(
			const std::string path,
			std::initializer_list<std::string> args,
			const std::string workdir,
			io::reader* _stdin, 
			io::writer* _stdout,
			io::writer* _stderr,
			const std::vector<std::string> &env)
			:_stdin(_stdin),_stdout(_stdout),_stderr(_stderr)
		{

			SECURITY_ATTRIBUTES sa;

			sa.bInheritHandle = TRUE;

			sa.lpSecurityDescriptor = NULL;

			sa.nLength = sizeof(SECURITY_ATTRIBUTES);

			HANDLE stdinHandler = GetStdHandle(STD_INPUT_HANDLE);
			HANDLE stdoutHandler = GetStdHandle(STD_OUTPUT_HANDLE);
			HANDLE stderrHandler = GetStdHandle(STD_ERROR_HANDLE);

			// create child stdin handler
			if(_stdin)
			{
				if(!::CreatePipe(&stdinHandler, &_stdinHandler, &sa, 0))
				{
					throw std::system_error(GetLastError(),std::system_category());
				}

				if(!::SetHandleInformation(_stdinHandler,HANDLE_FLAG_INHERIT,0))
				{
					throw std::system_error(GetLastError(), std::system_category());
				}
			}

			// create child stdout handler
			if (_stdout)
			{
				if (!::CreatePipe(&_stdoutHandler,&stdoutHandler, &sa, 0))
				{
					throw std::system_error(GetLastError(), std::system_category());
				}

				if (!::SetHandleInformation(_stdoutHandler, HANDLE_FLAG_INHERIT, 0))
				{
					throw std::system_error(GetLastError(), std::system_category());
				}
			}

			// create child stdout handler
			if (_stderr)
			{
				if (!::CreatePipe(&_stderrHandler, &stderrHandler, &sa, 0))
				{
					throw std::system_error(GetLastError(), std::system_category());
				}

				if (!::SetHandleInformation(_stderrHandler, HANDLE_FLAG_INHERIT, 0))
				{
					throw std::system_error(GetLastError(), std::system_category());
				}
			}

			convert conv;

			std::wstringstream stream;

			stream << L"\"" << conv.from_bytes(path) << L"\"";
	
			std::vector<wchar_t> cmdline;

			LPWSTR lpCommandLine = NULL;

			for (auto arg : args)
			{
				stream << " " << conv.from_bytes(arg);
			}

			auto str = stream.str();

			cmdline.assign(str.begin(), str.end());

			cmdline.push_back(L'\0');
			lpCommandLine = &cmdline[0];

			LPVOID lpEnvironment = NULL;

			STARTUPINFO si;

			ZeroMemory(&_processInfo, sizeof(_processInfo));

			ZeroMemory(&si, sizeof(STARTUPINFO));

			si.cb = sizeof(STARTUPINFO);
			si.hStdError = stderrHandler;
			si.hStdOutput = stdoutHandler;
			si.hStdInput = stdinHandler;
			si.dwFlags |= STARTF_USESTDHANDLES;

			if(!::CreateProcessW(
				NULL,
				lpCommandLine,
				NULL,
				NULL,
				TRUE,
				0, 
				lpEnvironment,
				workdir.empty()?NULL:conv.from_bytes(workdir).c_str(),
				&si,
				&_processInfo))
			{
				throw std::system_error(GetLastError(), std::system_category());
			}
		}

		int wait() 
		{
			if(WAIT_FAILED == WaitForSingleObject(_processInfo.hProcess,INFINITE))
			{
				throw std::system_error(GetLastError(), std::system_category());
			}

			DWORD exitCode;

			GetExitCodeProcess(_processInfo.hProcess, &exitCode);

			return exitCode;
		}

	private:
		PROCESS_INFORMATION		_processInfo;
		HANDLE					_stdinHandler;
		HANDLE					_stdoutHandler;
		HANDLE					_stderrHandler;
		io::reader				*_stdin;
		io::writer				*_stdout;
		io::writer				*_stderr;
	};


	command & command::start(std::initializer_list<std::string> args)
	{
		_process = new win32_process(_path, args, _workpath,_stdin, _stdout, _stderr, _env);

		return *this;
	}

}}

#endif //WIN32
