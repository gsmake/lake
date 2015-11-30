#ifndef LEMON_IO_RW_HPP
#define LEMON_IO_RW_HPP

#include <mutex>
#include <vector>
#include <cstdint>
#include <system_error>
#include <condition_variable>


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

		const std::vector<uint8_t> buff() const
        {
            return _buff;
        }

	private:
		size_t						_readoffset;
		std::vector<uint8_t>		_buff;
	};


	class pipe :public reader,public writer
	{
	public:
		
		pipe(size_t buffsize);

		~pipe();

		int read(buffer buff, std::error_code & err) final;

		int write(const_buffer buff, std::error_code & err) final;

	private:
		std::mutex					_mutex;
		std::condition_variable		_condition;
		std::vector<uint8_t>		_buff;
		size_t						_readoffset;
		size_t						_writeoffset;
	};
}}

#endif //LEMON_IO_RW_HPP