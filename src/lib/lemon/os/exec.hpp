#ifndef LEMON_OS_EXEC_HPP
#define LEMON_OS_EXEC_HPP

#include <lemon/io/readwriter.hpp>
#include <tuple>
#include <string>
#include <initializer_list>
#include <lemon/nocopy.hpp>


namespace lemon{ namespace exec{

	std::tuple<std::string,bool> lookup(const std::string & cmd);


	class command : public nocopy
	{
	public:

		command(const std::string & name);

		void start(std::initializer_list<std::string> args);

		/*
		 * wait command return
		 */
		int wait(); 

		/**
		 * set the stdin reader
		 */
		command & setstdin(io::reader *reader);
		/**
		 * set the stdout writer
		 */
		command & setstdout(io::writer * writer);
		/**
		 * set the child process env
		 */
		command & setenv(const std::vector<std::string> & env);
	private:
		const std::string					_path; // command fullpath
	};

}}

#endif //LEMON_OS_EXEC_HPP