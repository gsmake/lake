#include <lemoon/lemoon.hpp>
#include <lemoon/fs/fs.hpp>
#include <lemoon/log/log.hpp>
#include <lemoon/os/os.hpp>
#include <lemoon/regex/regex.hpp>


#include <sqlite/lsqlite3.h>

namespace lemoon {

	int luaopen_lemoon(lua_State *L)
	{
		luaL_openlibs(L);

		luaL_requiref(L, "__lemoon_fs", lemoon::fs::luaopen_fs, 1);

		luaL_requiref(L, "__lemoon_os", lemoon::os::luaopen_os, 1);

		luaL_requiref(L, "__lemoon_log", lemoon::log::luaopen_log, 1);

		luaL_requiref(L, "__lemoon_sqlite3", luaopen_lsqlite3,1);

		luaL_requiref(L, "__lemoon_regex", lemoon::regex::luaopen_regex,1);

		return 0;
	}
}