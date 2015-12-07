#ifndef LEMON_IO_ASYNC_BASIC_IRP_HPP
#define LEMON_IO_ASYNC_BASIC_IRP_HPP

namespace lemon{ namespace io{ namespace async{

    enum class irp_op {
        read,write,accept,connect
    };


    struct basic_irp
    {
        basic_irp                                        *next;

        basic_irp                                        **prev;

        irp_op                                           op;

        basic_irp(irp_op o):next(nullptr),prev(nullptr),op(o)
        { }

    };
}}}

#endif //LEMON_IO_ASYNC_BASIC_IRP_HPP