#include <iostream>
#include <lemon/test/runner.hpp>
#include <lemon/test/unit.hpp>


namespace lemon {namespace test{

    runner& runner::instance()
    {
        static runner global;

        return global;
    }

    void runner::run() {
        runner::instance().done();
    }

    void runner::done() {

        for(auto unit : _units)
        {
            try
            {
                unit->run();
                std::cout << "test(" << unit->name() << ")";
                std::cout << " ok" << std::endl;
            }
            catch(const std::exception &e)
            {
                std::cout << "test(" << unit->name() << ")";
                std::cout << " exec failed !!!!" << std::endl;
                std::cout <<"\terr  :" << e.what() << std::endl;
                std::cout <<"\tfile :" << unit->file() << "(" << unit->lines() << ")" << std::endl;
            }

        }
    }

    void runner::add(runnable *unit)
    {
        _units.push_back(unit);
    }
}}