#ifndef LEMON_TEST_MACRO_HPP
#define LEMON_TEST_MACRO_HPP

#include <lemon/test/unit.hpp>
#include <lemon/test/runner.hpp>

#define test_(name)\
class test_##name : public lemon::test::T {\
public:\
    test_##name(const std::string & name, const std::string & filename, int lines):T(name, filename,lines){}\
    void main() final;\
};\
namespace{\
    test_##name test_##name##_instance(#name,__FILE__,__LINE__); \
}\
void test_##name::main()

#define bench_(name)\
class bench_##name : public lemon::test::B {\
public:\
    bench_##name(const std::string & name, const std::string & filename, int lines):B(name, filename,lines){}\
    void main() final;\
};\
namespace{\
    bench_##name bench_##name##_instance(#name,__FILE__,__LINE__); \
}\
void bench_##name::main()

#endif //LEMON_TEST_MACRO_HPP