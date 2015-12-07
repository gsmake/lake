#include <mutex>

#include <lemon/uuid.hpp>

#include <lua/lua.hpp>

namespace lemoon{ namespace uuid{

    namespace {
        std::once_flag flag;

        lemon::uuids::random_generator *generator;
    }

    static void init()
    {
        generator = new lemon::uuids::random_generator();
    }

    int gen(lua_State *L)
    {
        std::call_once(flag,init);

        auto val = (*generator)();

        lua_pushstring(L,lemon::uuids::to_string(val).c_str());

        return 1;
    }

    static luaL_Reg funcs[] = {
        {"gen",gen},
        {NULL, NULL}
    };

    int luaopen_uuid(lua_State *L)
    {
        luaL_newlib(L,funcs);

        return 1;
    }
}}