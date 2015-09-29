#include <lemoon/lfs.h>

#ifdef WIN32

static int lfs_exists(lua_State *L);

static int lfs_isdir(lua_State *L);

static int lfs_mkdir(lua_State *L);

static int lfs_current_path(lua_State *L);

static int lfs_create_directory_symlink(lua_State *L);

static const luaL_Reg lfs_funcs[] = {
	{ "exists", lfs_exists },
	{ "isdir", lfs_isdir },
	{ "mkdir", lfs_mkdir },
	{ "current", lfs_current_path },
	{ "create_directory_symlink", lfs_create_directory_symlink },
	{ NULL, NULL }
};

LEMOON_PRIVATE EXTERN_C int lfs_new(lua_State *L) {

	lua_newtable(L);

	luaL_setfuncs(L, lfs_funcs, 0);

	return 1;
}

int lfs_exists(lua_State *L) {

	const char *filepath = luaL_checkstring(L, 1);

	if (INVALID_FILE_ATTRIBUTES == GetFileAttributesA(filepath)) {
		lua_pushboolean(L, 0);
	} else {
		lua_pushboolean(L, 1);
	}

	return 1;
}

static int lfs_isdir(lua_State *L) {
	const char *filepath = luaL_checkstring(L, 1);

	DWORD attrs = GetFileAttributesA(filepath);

	if (attrs == INVALID_FILE_ATTRIBUTES) {
		lua_pushboolean(L, 0);
		return 1;
	}

	if ((attrs & FILE_ATTRIBUTE_DIRECTORY) != 0) {
		lua_pushboolean(L, 1);
		return 1;
	}

	lua_pushboolean(L, 0);

	return 1;
}

static int lfs_mkdir(lua_State *L) {
	const char * path = luaL_checkstring(L, 1);

	if (0 == CreateDirectoryA(path, NULL)) {
		lemoonL_sysmerror(L, GetLastError(), "call CreateDirectoryA error");
	}

	return 0;
}

static int lfs_current_path(lua_State *L) {

	char buff[MAX_PATH];

	DWORD length = GetCurrentDirectoryA(MAX_PATH,buff);

	lua_pushlstring(L, buff, length);

	return 1;
}

static int lfs_create_directory_symlink(lua_State *L) {
	const char * target = luaL_checkstring(L, 1);
	const char * linkpath = luaL_checkstring(L, 2);

	if (0 == CreateSymbolicLinkA(linkpath, target, SYMBOLIC_LINK_FLAG_DIRECTORY)) {
		lemoonL_sysmerror(L, GetLastError(), "call CreateSymbolicLinkA error");
	}

	return 0;
}

#endif