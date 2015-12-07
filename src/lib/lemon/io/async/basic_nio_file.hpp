
#ifndef LEMON_IO_ASYNC_BASIC_NIO_HPP
#define LEMON_IO_ASYNC_BASIC_NIO_HPP

#include <fcntl.h>
#include <unistd.h>


#include <cerrno>
#include <utility>
#include <system_error>

#include <lemon/io/async/basic_file.hpp>


namespace lemon{ namespace io{ namespace async {

    template <typename Service>
    class basic_nio_file : public basic_file<Service>
    {
    public:

        typedef typename Service::fd_type               fd_type;


        basic_nio_file(Service &service, fd_type fd):basic_file<Service>(service,fd)
        {
            if(fcntl(fd, F_SETFL, fcntl(fd,F_GETFL,0) | O_NONBLOCK ) < 0)
            {
                close(fd);
                throw std::system_error(errno,std::system_category());
            }
        }

        ~basic_nio_file() {}
    };

}}}

#endif //LEMON_IO_ASYNC_BASIC_NIO_HPP