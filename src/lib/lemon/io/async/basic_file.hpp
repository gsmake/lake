#ifndef LEMON_IO_ASYNC_BASIC_FILE_HPP
#define LEMON_IO_ASYNC_BASIC_FILE_HPP

#include <system_error>

#include <lemon/config.h>
#include <lemon/io/io.hpp>
#include <lemon/nocopy.hpp>
#include <lemon/io/async/basic_irp.hpp>

namespace lemon{ namespace io{ namespace async{

    template <typename Service>
    class basic_file : private nocopy
    {

    public:

        typedef typename Service::fd_type               fd_type;
        typedef basic_irp                               irp_type;

        basic_file(Service &service,fd_type fd)
            :_service(&service),_fd(fd),_readQ(nullptr),_writeQ(nullptr)
        {
            std::error_code err;

            _service->attach(this,err);

            if (err)
            {
                throw std::system_error(err);
            }
        }

        virtual ~basic_file()
        {
            _service->detach(this);
        }

        fd_type get() const
        {
            return _fd;
        }

        bool null() const
        {
            return _fd == 0;
        }

        void reset(fd_type val)
        {
            _fd = val;
        }

        void add_irp_read (irp_type * irp) noexcept
        {
            add_irp(&_readQ,irp);
        }

        void remove_irp_read (irp_type *irp) noexcept
        {
            remove_irp(&_readQ,irp);
        }


        void add_irp_write (irp_type * irp) noexcept
        {
            add_irp(&_writeQ,irp);
        }

        void remove_irp_write (irp_type *irp) noexcept
        {
            remove_irp(&_writeQ,irp);
        }

#ifndef WIN32
        void io_process(irp_op op,const std::error_code & err) noexcept
        {

        }
#endif //WIN32

    private:

        static void add_irp(irp_type **header,irp_type *irp) noexcept
        {
            if(*header != nullptr)
            {
                irp->prev = (*header)->prev;
                irp->next = (*header);
                (*header)->prev = &irp->next;
            }
            else
            {
                (*header) = irp;

                irp->next = irp;

                irp->prev = &irp->next;
            }
        }

        static void remove_irp(irp_type **header,irp_type *irp) noexcept
        {
            if(irp == (*header))
            {
                (*header) = nullptr;
            }
            else
            {
                if(irp->next != nullptr)
                {
                    irp->next->prev = irp->prev;
                }

                if(irp->prev != nullptr)
                {
                    *irp->prev = irp->next;
                }
            }
        }

    private:
        Service                                     *_service;
        irp_type                                    *_readQ;
        irp_type                                    *_writeQ;
        fd_type                                     _fd;
    };

}}}

#endif //LEMON_IO_ASYNC_BASIC_FILE_HPP