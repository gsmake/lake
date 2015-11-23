#ifndef LEMON_IO_RW_HPP
#define LEMON_IO_RW_HPP

#include <vector>
#include <cstdint>
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


	class bytes :public reader,public writer
	{
	public:

		bytes();

		void reset();

		int read(buffer buff,std::error_code & err) final;
		
		int write(const_buffer buff,std::error_code & err) final;

	private:
		size_t						_readoffset;
		std::vector<uint8_t>		_buff;
	};
}}

#endif //LEMON_IO_RW_HPP