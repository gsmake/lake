#ifndef LEMON_IO_ASYNC_IO_SERVICE_KQUEUE_HPP
#define LEMON_IO_ASYNC_IO_SERVICE_KQUEUE_HPP

#include <sys/event.h>

#include <tuple>
#include <cerrno>
#include <chrono>
#include <system_error>
#include <unordered_map>

#include <lemon/nocopy.hpp>
#include <lemon/io/async/basic_irp.hpp>
#include <lemon/io/async/basic_file.hpp>


namespace lemon{ namespace io{ namespace async{

    class io_service_impl : private nocopy
    {
    public:
        typedef uintptr_t                       fd_type;

        io_service_impl()
        {
            _handler = kqueue();

            if(_handler == -1)
            {
                throw std::system_error(errno,std::system_category());
            }
        }

    protected:

        template <typename Duration>
        std::tuple<bool,fd_type,irp_op> dispatch_once(Duration duration, std::error_code & err)
        {
            using namespace std::chrono;

            struct kevent events[1];

            auto seconds_duration = duration_cast<seconds>(duration);

            auto nanoseconds_duration = duration_cast<nanoseconds>(duration - seconds_duration);

            timespec spec = { seconds_duration.count(), nanoseconds_duration.count() };

            auto result = kevent(_handler, NULL, 0, events, 1, &spec);


            if(result == -1)
            {
                err = std::error_code(errno,std::system_category());

                return std::tuple<bool,fd_type,irp_op>();
            }

            if(result == 0) return std::tuple<bool,fd_type,irp_op>();

            fd_type fd = events[0].ident;
            int16_t filter = events[0].filter;

            irp_op op;

            if((EVFILT_WRITE & filter) != 0)
            {
                op = irp_op::write;
            }

            if((EVFILT_READ & filter) != 0)
            {
                op = irp_op::read;
            }

            if(EV_ERROR == events[0].fflags)
            {
                err = std::error_code((int) events[0].data, std::system_category());

                return std::make_tuple(true,fd,op);
            }

            return std::make_tuple(true,fd,op);
        }

        /**
         * attach new file handle with this io service
         */
        template <typename Service>
        void attach(basic_file<Service> * file,std::error_code & err) noexcept
        {
            struct kevent changes[2];
            EV_SET(&changes[0], file->get(), EVFILT_READ, EV_ADD|EV_CLEAR|EV_EOF, 0, 0, NULL);
            EV_SET(&changes[1], file->get(), EVFILT_WRITE, EV_ADD|EV_CLEAR|EV_EOF, 0, 0, NULL);

            if(kevent(_handler, changes, sizeof(changes)/sizeof(struct kevent), NULL, 0, NULL) == -1)
            {
                err = std::error_code(errno,std::system_category());
            }
        }

    private:

        int                                         _handler;
    };

}}}

#endif //LEMON_IO_ASYNC_IO_SERVICE_KQUEUE_HPP