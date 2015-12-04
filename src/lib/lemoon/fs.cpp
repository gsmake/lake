#include <iostream>

#include <lua/lua.hpp>
#include <lemoon/config.h>
#include <lemon/fs/fs.hpp>

namespace lemoon{namespace fs{


    int pwd(lua_State *L)
    {
        if(lua_type(L,1) == LUA_TNONE || lua_type(L,1) == LUA_TNIL)
        {
            std::error_code err;
            auto curr = lemon::fs::current_directory(err);

            if (err)
            {
                return luaL_error(L,"[lemoon.fs] call lemon::fs::current_directory error :%s",err.message().c_str());
            }

            lua_pushstring(L,curr.c_str());

            return 1;
        }
        else
        {
            const char * path = luaL_checkstring(L,1);

            std::error_code err;

            lemon::fs::set_current_directory(path,err);

            if (err)
            {
                return luaL_error(L,"[lemoon.fs] call lemon::fs::set_current_directory error :%s",err.message().c_str());
            }

        }

        return 0;
    }

    int exists(lua_State *L)
    {
        if(lemon::fs::exists(luaL_checkstring(L,1)))
        {
            lua_pushboolean(L,1);
        }
        else
        {
            lua_pushboolean(L,0);
        }

        return 1;
    }

    int is_dir(lua_State *L)
    {
        if(lemon::fs::is_directory(luaL_checkstring(L,1)))
        {
            lua_pushboolean(L,1);
        }
        else
        {
            lua_pushboolean(L,0);
        }

        return 1;
    }

    int mkdir(lua_State *L)
    {
        std::error_code err;

        if(lua_type(L,2) == LUA_TBOOLEAN && lua_toboolean(L,2))
        {
            lemon::fs::create_directories(luaL_checkstring(L,1),err);

            if(err)
            {
                return luaL_error(L,"[lemoon.fs] call emon::fs::create_directories error :%s",err.message().c_str());
            }

            return 0;
        }

        lemon::fs::create_directory(luaL_checkstring(L,1),err);


        if (err)
        {
            return luaL_error(L,"[lemoon.fs] call emon::fs::create_directory error :%s",err.message().c_str());
        }

        return 0;
    }

    int rm(lua_State *L)
    {
        std::error_code err;

        auto path = luaL_checkstring(L,1);

        if(lua_type(L,2) != LUA_TNONE && lua_type(L,2) != LUA_TNIL)
        {
            if(lemon::fs::is_directory(path))
            {
                lemon::fs::remove_directories(path,err);

                if(err)
                {
                    return luaL_error(L,"[lemoon.fs] call emon::fs::remove_directories error :%s",err.message().c_str());
                }

                return 0;
            }
        }

        lemon::fs::remove_file(luaL_checkstring(L,1),err);

        if(err)
        {
            return luaL_error(L,"[lemoon.fs] call emon::fs::remove_file error :%s",err.message().c_str());
        }

        return 0;
    }

    int abs(lua_State *L)
    {
        auto fullPath = lemon::fs::abs(luaL_checkstring(L,1));

        lua_pushstring(L,fullPath.c_str());

        return 1;
    }

    int symlink(lua_State *L)
    {
        std::error_code err;

        lemon::fs::create_symlink(luaL_checkstring(L,1),luaL_checkstring(L,2),err);

        if(err)
        {
            return luaL_error(L,"[lemoon.fs] call emon::fs::create_symlink error :%s",err.message().c_str());
        }

        return 0;
    }

    int list(lua_State *L)
    {
        luaL_checktype(L, 2, LUA_TFUNCTION);

        std::error_code err;

        auto entries = lemon::fs::read_directory(luaL_checkstring(L,1),err);

        if(err)
        {
            return luaL_error(L,"[lemoon.fs] call emon::fs::read_directory error :%s",err.message().c_str());
        }

        for(auto entry : entries)
        {
            lua_pushvalue(L, 2);

            lua_pushstring(L, entry.c_str());

            if (0 != lua_pcall(L, 1, 0, 0))
            {
                return luaL_error(L,"[lemoon.fs] invoke list callback error :%s",lua_tostring(L,-1));
            }
        }

        return 0;
    }


    static luaL_Reg fs[] = {
        {"list",list},
        {"symlink",symlink},
        {"abs",abs},
        {"dir",pwd},
        {"isdir",is_dir},
        {"exists",exists},
        {"mkdir",mkdir},
        {"rm",rm},
        {NULL, NULL}
    };

    int luaopen_fs(lua_State *L)
    {
        luaL_newlib(L,fs);
        return 1;
    }
}}