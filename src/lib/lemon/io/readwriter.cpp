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
}}