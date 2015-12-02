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
		virtual size_t read(buffer buff,std::error_code & err) = 0;
		virtual ~ reader() {}
	};

    class reader_close : public reader
    {
    public:
        virtual void close() = 0;
    };

	class writer : public nocopy
	{
	public:
		virtual size_t write(const_buffer buff, std::error_code & err) = 0;
		virtual ~ writer() {}
	};


    class writer_close : public writer
    {
    public:
        virtual  void close() = 0;
    };


	class bytes :public reader,public writer
	{
	public:

		bytes();

		void reset();

        size_t read(buffer buff,std::error_code & err) final;

        size_t write(const_buffer buff,std::error_code & err) final;

		const std::vector<uint8_t> buff() const
        {
            return _buff;
        }

	private:
		size_t						_readoffset;
		std::vector<uint8_t>		_buff;
	};


	class pipe :public reader_close,public writer_close
	{
	public:
		
		pipe(size_t buffsize);

		~pipe();

        size_t read(buffer buff, std::error_code & err) final;

        size_t write(const_buffer buff, std::error_code & err) final;

        void close() final;

	private:
        bool                        _exit;
		std::mutex					_mutex;
		std::condition_variable		_condition;
		std::vector<uint8_t>		_buff;
		size_t						_readoffset;
		size_t						_writeoffset;
	};
}}

#endif //LEMON_IO_RW_HPP