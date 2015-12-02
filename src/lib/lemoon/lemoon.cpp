#include <lemoon/lemoon.hpp>
#include <sqlite/lsqlite3.h>

namespace lemoon { namespace fs{
    int luaopen_fs(lua_State *L);
}}

namespace lemoon { namespace os{
    int luaopen_os(lua_State *L);
}}

namespace lemoon { namespace log{
    int luaopen_log(lua_State *L);
}}

namespace lemoon { namespace regex{
    int luaopen_regex(lua_State *L);
}}

namespace lemoon { namespace uuid{
	int luaopen_uuid(lua_State *L);
}}

namespace lemoon {

    static luaL_Reg funcs[] = {
        {"fs",lemoon::fs::luaopen_fs},
        {"sys",lemoon::os::luaopen_os},
        {"log",lemoon::log::luaopen_log},
        {"sqlite3",luaopen_lsqlite3},
        {"regex",lemoon::regex::luaopen_regex},
		{"uuid",lemoon::uuid::luaopen_uuid},
        {NULL, NULL}
    };

	int luaopen_lemoon(lua_State *L)
	{
		luaL_openlibs(L);

        luaL_newlib(L,funcs);

		return 1;
	}
}