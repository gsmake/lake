#ifndef LEMON_IO_ASYNC_BASIC_STREAM_HPP
#define LEMON_IO_ASYNC_BASIC_STREAM_HPP

#include <utility>

namespace lemon{ namespace io{ namespace async {

    template <
        typename Service,
        template<typename T> class File
    >
    class basic_stream : public File<Service>
    {
    public:

        typedef typename Service::fd_type   fd_type;

        basic_stream(Service &service, fd_type fd):File<Service>(service,fd)
        {

        }
    };

}}}

#endif //LEMON_IO_ASYNC_BASIC_STREAM_HPP