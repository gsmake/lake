#ifndef LEMON_OS_PROCESS_HPP
#define LEMON_OS_PROCESS_HPP

#include <lemon/config.h>
#include <lemon/nocopy.hpp>

namespace lemon{ namespace os{
	class process : nocopy
	{
	public:
		virtual int wait() = 0;
	};
}}


#endif //LEMON_OS_PROCESS_HPP