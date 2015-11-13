#include <lemon/test/unit.hpp>


namespace lemon{ namespace test{

    T::T(const std::string &&name, std::function<void(const T&)> func)
        :_name(name),_func(func)
    {

    }

    void T::run()
    {
        _func(this);
    }

    const std::string& T::name() const
    {
        return _name;
    }
}}