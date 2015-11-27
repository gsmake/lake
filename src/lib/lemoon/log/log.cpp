#include <lemoon/log/log.hpp>
#include <lemon/log/log.hpp>
#include <lemon/fs/filepath.hpp>
#include <iostream>

namespace lemoon{namespace log{

    int lua_get(lua_State *L)
    {
        auto source = lemon::log::get(luaL_checkstring(L,1));

        lua_pushlightuserdata(L,source);

        return 1;
    }

    int lua_log(lua_State *L)
    {
        luaL_checktype(L,1,LUA_TLIGHTUSERDATA);

        auto source = (lemon::log::logger*)lua_touserdata(L,1);

        auto msg = luaL_checkstring(L,3);

        lua_Debug debug;

        lua_getstack(L,2, &debug);

        lua_getinfo(L,"S", &debug);

        auto file = lemon::filepath::path(debug.source + 1).leaf();

        lemon::log::log(source,(lemon::log::level)luaL_checkinteger(L,2),file.c_str(),debug.currentline,msg);

        return 0;
    }

    int lua_flush(lua_State *L)
    {
        lemon::log::stop();

        return 0;
    }

    static luaL_Reg funcs[] = {
        {"get",lua_get},
        {"log",lua_log},
        {"exit",lua_flush},
        {NULL, NULL}
    };

    int luaopen_log(lua_State *L)
    {
        luaL_newlib(L,funcs);
        return 1;
    }
}}