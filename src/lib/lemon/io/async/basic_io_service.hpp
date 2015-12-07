#ifndef LEMON_IO_ASYNC_BASIC_IO_SERVICE_HPP
#define LEMON_IO_ASYNC_BASIC_IO_SERVICE_HPP

#include <tuple>
#include <algorithm>
#include <unordered_map>

#include <lemon/config.h>
#include <lemon/nocopy.hpp>
#include <lemon/io/async/basic_file.hpp>

namespace lemon{ namespace io{ namespace async{

    template <typename Impl,typename Mutex>
    class basic_io_service : public Impl
    {
    public:
        typedef typename Impl::fd_type              fd_type;
        typedef basic_io_service<Impl,Mutex>        io_service_type;
        typedef basic_file<io_service_type>         file_type;

        template <typename Duration>
        void dispatch_once(Duration timeout, std::error_code & err) noexcept
        {
#ifndef WIN32
            auto result = Impl::dispatch_once(timeout,err);

            if(std::get<0>(result))
            {
                std::lock_guard<Mutex> locker(_mutex);

                auto iter = _files.find(std::get<1>(result));

                if(iter != _files.end())
                {
                    iter->second->io_process(std::get<2>(result),err);
                }

                if(err) err = std::error_code();
            }
#endif //WIN32
        }

        template <typename Duration>
        void dispatch_once(Duration timeout)
        {
            std::error_code err;
            dispatch_once(timeout,err);

            if (err)
            {
                throw std::system_error(err);
            }
        }

        void attach(file_type * file,std::error_code & err) noexcept
        {
            Impl::attach(file,err);

            if(!err)
            {
                std::lock_guard<Mutex> locker(_mutex);

                _files[file->get()] = file;
            }

        }

        void detach(file_type * file)
        {
            std::lock_guard<Mutex> locker(_mutex);

            _files.erase(file->get());
        }

    private:
        Mutex                                           _mutex;
        std::unordered_map<fd_type,file_type*>          _files;
    };

}}}

#endif // LEMON_IO_ASYNC_BASIC_IO_SERVICE_HPP