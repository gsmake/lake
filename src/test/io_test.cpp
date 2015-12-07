#include <lemon/io/io.hpp>
#include <lemon/test/test.hpp>


using namespace lemon::io;
using namespace std::chrono;

test_(pipe)
{
    lemon::io::io_service io;

    lemon::io::pipe pipe(io);

    pipe.in();

    pipe.out();
}


test_(duration)
{
    auto duration = seconds(1) + nanoseconds(101);

    auto duration_seconds = duration_cast<seconds>(duration);

    test_assert(duration_seconds.count() == 1);

    auto duration_nano = duration_cast<nanoseconds>(duration - duration_seconds);

    test_assert(duration_nano.count() == 101);
}