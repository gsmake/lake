//
// Created by liyang on 15/11/13.
//

#include <sstream>
#include <iostream>
#include <lemoon/lemoon.hpp>
#include <lemon/fs/fs.hpp>
#include <lemon/os/sysinfo.hpp>
#include <lemon/log/log.hpp>

int pmain(lua_State *L)
{

    luaL_openlibs(L);

    luaL_requiref(L,"lemoon",lemoon::luaopen_lemoon,1);

    auto path = lemon::os::getenv("GSMAKE_HOME");

    if(!std::get<1>(path))
    {
        return luaL_error(L,"GSMAKE_HOME env not found");
    }

    auto home = lemon::filepath::path(std::get<0>(path)).slash();

    std::stringstream stream;

	stream << "package.path = '" << home << "/share/?.lua;" << home << "/share/?/init.lua'";

    if(luaL_dostring(L,stream.str().c_str()))
    {
        return luaL_error(L,lua_tostring(L,-1));
    }


    auto mainFile = lemon::filepath::path(home + "/share/main.lua");

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

		lemonE(lemon::log::get("lake"),"panic:\n\t%s", lua_tostring(L, -1));
    }

	lemon::log::close();

    lua_close(L);
}