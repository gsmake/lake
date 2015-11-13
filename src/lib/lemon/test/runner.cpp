#include <lemon/test/runner.hpp>

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

    }

    void runner::add(runnable *unit) {
        _units.push_back(unit);
    }
}}