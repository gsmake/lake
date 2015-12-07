#ifndef LEMON_IO_ASYNC_BASIC_PIPE_POSIX_HPP
#define LEMON_IO_ASYNC_BASIC_PIPE_POSIX_HPP

#include <string>
#include <utility>
#include <lemon/nocopy.hpp>
#include <lemon/io/async/basic_stream.hpp>
#include <lemon/io/async/basic_nio_file.hpp>

namespace lemon{ namespace io{ namespace async {

    template <typename Service>
    class basic_pipe_stream : public basic_stream<Service,basic_nio_file>
    {
    public:

        typedef basic_stream<Service,basic_nio_file> base_type;

        typedef typename Service::fd_type            fd_type;

        basic_pipe_stream(Service & service, fd_type fd):base_type(service,fd)
        {

        }

        ~basic_pipe_stream()
        {
            close();
        }

        void close()
        {
            if(base_type::get() != 0)
            {
                ::close(base_type::get());
            }
        }
    };

    template <typename Service>
    class basic_pipe : private nocopy
    {
    public:
        typedef basic_pipe_stream<Service> stream_type;

        basic_pipe(Service &service)
        {
            int fds[2];

            if(0 != pipe(fds))
            {
                throw std::system_error(errno,std::system_category());
            }

            _in = new stream_type(service,fds[0]);

            _out = new stream_type(service,fds[1]);
        }

        ~basic_pipe()
        {
            delete _in;
            delete _out;
        }

        stream_type & in() const
        {
            return *_in;
        }

        stream_type & out() const
        {
            return *_out;
        }

    private:
        stream_type                     *_in;
        stream_type                     *_out;
    };

}}}

#endif //LEMON_IO_ASYNC_BASIC_PIPE_POSIX_HPP