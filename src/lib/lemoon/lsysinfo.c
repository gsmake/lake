#include <lemoon/lsysinfo.h>

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

static int lsys_osname(lua_State *L);

static const luaL_Reg lsysinfo_funcs[] = {
	{"osname",lsys_osname},
	{ NULL, NULL }
};


LEMOON_PRIVATE EXTERN_C lsysinfo(lua_State *L) {
	lua_newtable(L);

	luaL_setfuncs(L, lsysinfo_funcs, 0);

	return 1;
}

static int lsys_osname(lua_State *L) {

#ifdef WIN32
#ifdef _WIN64
	lua_pushstring(L, "win64");
#else
	lua_pushstring(L, "win32");
#endif
#elif defined(__linux)
	lua_pushstring(L,"linux");
#elif defined(__sun)
	lua_pushstring(L, "solaris");
#elif defined(__hppa)
	lua_pushstring(L, "hpux");
#elif defined(_AIX)
	lua_pushstring(L, "aix");
#elif defined(__APPLE__)
#if TARGET_IPHONE_SIMULATOR
	lua_pushstring(L, "ios-simulator");
#elif TARGET_OS_IPHONE
	lua_pushstring(L, "ios");
#elif TARGET_OS_MAC
	lua_pushstring(L, "osx");
#else
	lua_pushstring(L, "unknown-apple");
#endif
#endif //WIN32
	return 1;
}