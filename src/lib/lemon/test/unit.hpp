#ifndef LEMON_TEST_UNIT_HPP
#define LEMON_TEST_UNIT_HPP

#include <string>
#include <functional>
#include <lemon/nocopy.hpp>

namespace lemon { namespace test {
    /**
    * The runnable interface defined the test/benchmark unit
    */
    class runnable : private nocopy
    {
    public:
        /**
         * runner call this function to invoke current test/bench unit
         */
        virtual void run() = 0;

        virtual const std::string & name() const = 0;
    };


    /**
     * the test unit class hold the test function object and test name
     */
    class T : public runnable
    {
    public:

        T(const std::string && name,std::function<void(const T&)> func);

        void run() final;

        /**
         * get the test name;
         */
        const std::string & name() const final;

    private:

        std::string                     _name;

        std::function<void(const T&)>   _func; /* the test function object */
    };
}}

#endif //LEMON_TEST_UNIT_HPP