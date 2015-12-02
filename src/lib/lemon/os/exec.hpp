#ifndef LEMON_OS_EXEC_HPP
#define LEMON_OS_EXEC_HPP

#include <lemon/io/readwriter.hpp>
#include <vector>
#include <tuple>
#include <string>
#include <initializer_list>
#include <lemon/nocopy.hpp>
#include <lemon/os/process.hpp>

namespace lemon{ namespace exec{

	std::tuple<std::string,bool> lookup(const std::string & cmd);


	class command final: public nocopy
	{
	public:
		command(const std::string & name);

		~command();
		/**
		 * set the stdin reader
		 */
		command & setstdin(io::reader_close *reader);
		/**
		 * set the stdout writer
		 */
		command & setstdout(io::writer_close * writer);

		/**
		* set the stderr writer
		*/
		command & setstderr(io::writer_close * writer);
		/**
		 * set the child process env
		 */
		command & setenv(const std::vector<std::string> & env);

		int wait();

		command & start(std::vector<std::string> args);

		command & start(std::initializer_list<std::string> args)
		{
			start(std::vector<std::string>(args));

			return *this;
		}

		command & setdir(const std::string & dir);

	private:
		std::string							_path; // command fullpath
		io::reader_close*					_stdin;
		io::writer_close*					_stdout;
		io::writer_close*					_stderr;
		std::vector<std::string>			_env;
		std::string							_workpath;
		os::process*						_process;
	};
}}

#endif //LEMON_OS_EXEC_HPP