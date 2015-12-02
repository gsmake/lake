#include <iostream>
#include <lemon/fs/fs.hpp>
#include <lemon/test/test.hpp>
#include <lemon/os/sysinfo.hpp>
#include <lemoon/lemoon.hpp>

namespace lemoon{ namespace test{

    class T
    {
    public:
        T(const std::string & name)
        {
            L = luaL_newstate();

            luaL_openlibs(L);

            luaL_requiref(L,"lemoon",lemoon::luaopen_lemoon,1);

            auto path = lemon::os::getenv("GSMAKE_HOME");

            if(!std::get<1>(path))
            {
                luaL_error(L,"GSMAKE_HOME env not found");
            }

            auto home = lemon::filepath::path(std::get<0>(path)).slash();

            std::stringstream stream;

            stream << "package.path = package.path ..';" << home << "/runtimes/?.lua;" << home << "/test/?.lua'";

            if(luaL_dostring(L,stream.str().c_str()))
            {
                luaL_error(L,lua_tostring(L,-1));
            }


            auto mainFile = lemon::filepath::path(home + "/test/" + name);

            if(luaL_dofile(L,mainFile.slash().c_str()))
            {
                luaL_error(L,lua_tostring(L,-1));
            }
        }

        ~T()
        {
            lua_close(L);
        }

    private:

        lua_State *L;
    };
}}


test_(fs) {
    //lemoon::test::T("fs.lua");
}


test_(os) {
    //lemoon::test::T("sys.lua");
}
