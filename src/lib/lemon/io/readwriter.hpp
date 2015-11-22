#ifndef LEMON_IO_RW_HPP
#define LEMON_IO_RW_HPP

#include <system_error>
#include <lemon/config.h>
#include <lemon/nocopy.hpp>
#include <lemon/io/buff.hpp>
namespace lemon{ namespace io{


	class reader : public nocopy
	{ 
	public:
		virtual int read(buffer buff,std::error_code & err) = 0;
		virtual ~ reader() {}
	};

	class writer : public nocopy
	{
	public:
		virtual int write(const_buffer buff, std::error_code & err) = 0;
		virtual ~ writer() {}
	};
}}

#endif //LEMON_IO_RW_HPP