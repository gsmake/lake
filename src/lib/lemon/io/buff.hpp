#ifndef LEMON_IO_BUFF_HPP
#define LEMON_IO_BUFF_HPP

namespace lemon{ namespace io{

	struct buffer
	{
		void			*data;
		size_t			length;
	};

	struct const_buffer
	{
		const void		*data;
		size_t			length;
	};
}}

#endif //LEMON_IO_BUFF_HPP