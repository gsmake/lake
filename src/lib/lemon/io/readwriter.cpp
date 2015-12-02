#include <lemon/io/readwriter.hpp>

namespace lemon {namespace io{

	bytes::bytes()
		:_readoffset(0)
	{

	}

	void bytes::reset()
	{
		_readoffset = 0;
		_buff.clear();
	}

    size_t bytes::read(buffer buff, std::error_code & err)
	{
		size_t readlen = buff.length;

		if(_buff.size() < buff.length + _readoffset)
		{
			readlen = _buff.size() - _readoffset;

			err = std::make_error_code(std::errc::result_out_of_range);
		}

		memcpy(buff.data, &_buff[_readoffset], readlen);

		_readoffset += readlen;

		return readlen;
	}

    size_t bytes::write(const_buffer buff, std::error_code & err)
	{
		size_t writelen = buff.length;

		_buff.assign(&((const uint8_t*)buff.data)[0], &((const uint8_t*)buff.data)[buff.length]);

		return writelen;
	}

	pipe::pipe(size_t buffsize)
		:_readoffset(0),_writeoffset(0),_exit(false)
	{
		_buff.resize(buffsize);
	}

	pipe::~pipe()
	{

	}

    size_t pipe::read(buffer buff, std::error_code & err)
	{
		std::unique_lock<std::mutex> lock(_mutex);

        if(_exit)
        {
            err = std::make_error_code(std::errc::broken_pipe);

            return 0;
        }

		while(_readoffset == _writeoffset)
		{
			_condition.wait(lock);

            if(_exit)
            {
                err = std::make_error_code(std::errc::broken_pipe);

                return 0;
            }
		}

		auto readoffset = _readoffset % _buff.size();

		auto readsize = buff.length;

		if(_writeoffset - _readoffset < buff.length)
		{
			readsize = _writeoffset - _readoffset;
		}

		if(readsize + readoffset > _buff.size())
		{
			auto readOne = _buff.size() - readoffset;

			memcpy(buff.data,&_buff[readoffset],readOne);

			memcpy((uint8_t*)buff.data + readOne,&_buff[0],readsize - readOne);
		}
		else
		{
			memcpy(buff.data, &_buff[readoffset], readsize);
		}

        _readoffset += readsize;

		_condition.notify_all();

		return readsize;
	}

    size_t pipe::write(const_buffer buff, std::error_code & err)
	{
		std::unique_lock<std::mutex> lock(_mutex);

        if(_exit)
        {
            err = std::make_error_code(std::errc::broken_pipe);

            return 0;
        }

		while(_writeoffset - _readoffset == _buff.size())
		{
			_condition.wait(lock);

            if(_exit)
            {
                err = std::make_error_code(std::errc::broken_pipe);

                return 0;
            }
		}

		auto writeoffset = _writeoffset % _buff.size();

		auto writesize = buff.length;

		if(_writeoffset + writesize - _readoffset > _buff.size())
		{
			writesize = _buff.size() + _readoffset - _writeoffset;
		}

		if (writesize + _writeoffset > _buff.size())
		{
			auto writeOne = _buff.size() - writeoffset;

			memcpy(&_buff[writeoffset],buff.data,writeOne);

			memcpy(&_buff[0],(uint8_t*)buff.data + writeOne,writesize - writeOne);
		}
		else
		{
			memcpy(&_buff[writeoffset], buff.data, writesize);
		}

        _writeoffset += writesize;

		_condition.notify_all();

		return writesize;
	}

	void pipe::close()
	{
        std::unique_lock<std::mutex> lock(_mutex);

        if(!_exit){
            _exit = true;
            _condition.notify_all();
        }
	}
}}