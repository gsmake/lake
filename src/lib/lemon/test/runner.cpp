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
            unit->run();
        }
    }

    void runner::add(runnable *unit)
    {
        _units.push_back(unit);
    }
}}