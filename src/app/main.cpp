//
// Created by liyang on 15/11/13.
//

#include <sstream>
#include <iostream>
#include <lemon/fs/fs.hpp>
#include <lemoon/fs/fs.hpp>
#include <lemoon/os/os.hpp>
#include <lemon/os/sysinfo.hpp>


int pmain(lua_State *L)
{

    luaL_openlibs(L);

    luaL_requiref(L, "__lemoon_fs", lemoon::fs::luaopen_fs, 1);

    luaL_requiref(L, "__lemoon_os", lemoon::os::luaopen_os, 1);

    auto path = lemon::os::getenv("GSMAKE_HOME");

    if(!std::get<1>(path))
    {
        return luaL_error(L,"GSMAKE_HOME env not found");
    }

    auto home = lemon::filepath::path(std::get<0>(path)).slash();

    std::stringstream stream;

    stream << "package.path = package.path ..';" << home << "/runtimes/?.lua'";

    if(luaL_dostring(L,stream.str().c_str()))
    {
        return luaL_error(L,lua_tostring(L,-1));
    }


    auto mainFile = lemon::filepath::path(home + "/runtimes/main.lua");

    if(luaL_dofile(L,mainFile.slash().c_str()))
    {
        return luaL_error(L,lua_tostring(L,-1));
    }

    return 0;
}

int main() {

    lua_State *L = luaL_newstate();

    lua_pushcfunction(L, pmain);

    if (0 != lua_pcall(L, 0, 0, 0)) {
        std::cout << "panic:\n" << lua_tostring(L,-1) << std::endl;
    }

    lua_close(L);
}