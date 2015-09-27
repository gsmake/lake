
#include <filesystem>
#include <lemoon/lfs.h>



static int lfs_dir_ls(lua_State *L);

static int lfs_exists(lua_State *L);

static int lfs_absolute(lua_State *L);

static int lfs_current_path(lua_State *L);

static const luaL_Reg lfs_funcs[] =
{
	{"ls",lfs_dir_ls},
	{"exists",lfs_exists},
	{ "absolute",lfs_absolute },
	{ "current", lfs_current_path },
	{ NULL,NULL }
};


LEMOON_PRIVATE EXTERN_C int lfs_new(lua_State *L)
{
	lua_newtable(L);

	luaL_setfuncs(L, lfs_funcs, 0);

	return 1;
}

int lfs_dir_ls(lua_State *L)
{
	return 0;
}




int lfs_exists(lua_State *L)
{
	const char * path = luaL_checkstring(L,1);

	if (std::experimental::filesystem::exists(path)) 
	{
		lua_pushboolean(L, 1);
	}
	else
	{
		lua_pushboolean(L, 0);
	}

	return 1;
}


static int lfs_absolute(lua_State *L)
{
	const char * path = luaL_checkstring(L, 1);

	std::string fullpath = std::experimental::filesystem::absolute(path).string();

	lua_pushstring(L, fullpath.c_str());

	return 1;
}


static int lfs_current_path(lua_State *L)
{
	lua_pushstring(L, std::experimental::filesystem::current_path().string().c_str());

	return 1;
}