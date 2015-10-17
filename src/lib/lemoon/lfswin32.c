#include <lemoon/lfs.h>

#ifdef WIN32

static int lfs_exists(lua_State *L);

static int lfs_isdir(lua_State *L);

static int lfs_mkdir(lua_State *L);

static int lfs_current_path(lua_State *L);

static int lfs_set_current_path(lua_State *L);

static int lfs_fullpath(lua_State *L);

static int lfs_create_directory_symlink(lua_State *L);

static int lfs_rmdir(lua_State *L);

static int lfs_rmfile(lua_State *L);

static int lfs_ls(lua_State *L);

static const luaL_Reg lfs_funcs[] = {
	{ "exists", lfs_exists },
	{ "isdir", lfs_isdir },
	{ "mkdir", lfs_mkdir },
	{ "current", lfs_current_path },
	{ "setcurrent", lfs_set_current_path },
	{ "fullpath", lfs_fullpath },
	{ "rmdir", lfs_rmdir },
	{ "rmfile", lfs_rmfile },
	{ "ls", lfs_ls },
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

static int lfs_set_current_path(lua_State *L) {
	const char * target = luaL_checkstring(L, 1);
	if(!SetCurrentDirectoryA(target)){
		lemoonL_sysmerror(L, GetLastError(),"set current path to (%s) error",target);
	}

	return 0;
}

static int lfs_create_directory_symlink(lua_State *L) {
	const char * target = luaL_checkstring(L, 1);
	const char * linkpath = luaL_checkstring(L, 2);

	if (0 == CreateSymbolicLinkA(linkpath, target, SYMBOLIC_LINK_FLAG_DIRECTORY)) {
		lemoonL_sysmerror(L, GetLastError(), "call CreateSymbolicLinkA error");
	}

	return 0;
}

static int lfs_rmdir(lua_State *L) {
	const char * target = luaL_checkstring(L, 1);

	if(0 == RemoveDirectoryA(target)) {
		lemoonL_sysmerror(L,GetLastError(),"rm directory error :%s",target);
	}

	return 0;
}

static int lfs_rmfile(lua_State *L) {
	
	const char * target = luaL_checkstring(L, 1);

	for (;;) {
		if (0 == DeleteFileA(target)) {

			if (GetLastError() == ERROR_ACCESS_DENIED) {
				DWORD attrs = GetFileAttributesA(target);
				attrs &= ~FILE_ATTRIBUTE_READONLY;
				SetFileAttributesA(target, attrs);
				continue;
			}

			lemoonL_sysmerror(L, GetLastError(), "rm file error :%s", target);
		}

		break;
	}

	

	return 0;
}

static int lfs_ls(lua_State *L) {
	WIN32_FIND_DATAA context;

	HANDLE handler = NULL;

	const char * target = luaL_checkstring(L, 1);

	char path[1024] = { 0 };

	sprintf_s(path, sizeof(path), "%s/*", target);

	luaL_checktype(L, 2, LUA_TFUNCTION);

	if((handler = FindFirstFileA(path,&context)) == INVALID_HANDLE_VALUE){
		return lemoonL_sysmerror(L, GetLastError(),"list directory error :%s", target);
	}

	do {
		
		lua_pushvalue(L, 2);

		lua_pushstring(L, context.cFileName);

		if (0 != lua_pcall(L, 1, 0, 0)) {
			FindClose(handler);
			lemoonL_error(L, "ls callback error :%s", lua_tostring(L, -1));
		}
	} while (FindNextFileA(handler,&context));

	FindClose(handler);

	return 0;
}

static int lfs_fullpath(lua_State *L) {

	DWORD length = GetFullPathNameA(luaL_checkstring(L,1),0,NULL,NULL);

	if (length == 0) {
		return lemoonL_sysmerror(L, GetLastError(), "call GetFullPathName error");
	}

	char * buff = (char*)malloc(length);

	GetFullPathNameA(luaL_checkstring(L, 1), length, buff, NULL);

	lua_pushlstring(L, buff, length);

	free(buff);

	return 1;
}

#endif