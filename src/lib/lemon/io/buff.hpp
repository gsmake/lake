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

		const_buffer(buffer buff)
		{
			data	= buff.data;
			length	= buff.length;
		}
	};

    template<size_t N>
	inline buffer buff(char (&source)[N])
    {
        return {source,N};
    }
}}

#endif //LEMON_IO_BUFF_HPP