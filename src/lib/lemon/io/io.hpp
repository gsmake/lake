#ifndef LEMON_IO_HPP
#define LEMON_IO_HPP

#ifndef WIN32
#include <lemon/io/async/pipe_posix.hpp>
#endif //WIN32

#if defined(__APPLE__)
#include <lemon/io/async/io_service_kqueue.hpp>
#endif

#include <mutex>

#include <lemon/io/async/mutex_nothing.hpp>
#include <lemon/io/async/basic_io_service.hpp>

namespace lemon{ namespace io{

    template <typename Mutex>
    using basic_io_service = async::basic_io_service<async::io_service_impl,Mutex>;

    using io_service = basic_io_service<std::mutex>;

    template <typename Mutex>
    using basic_pipe  = async::basic_pipe<basic_io_service<Mutex>>;

    using pipe = async::basic_pipe<io_service>;
}}


#endif //LEMON_IO_HPP