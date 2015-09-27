#include <lemoon/lemoon.h>

#define ENV "LAKE_HOME"

static int lake_pmain(lua_State *L)
{
	luaL_openlibs(L);

	luaL_requiref(L, "lemoon", luaopen_lemoon, 1);

	const char * path = getenv(ENV);

	if (path == 0) {
		lemoonL_error(L, "env " ENV " not found");
	}

	if (LEMOON_RUNTIME_ERROR == lemoonL_dostring(L, "package.path = package.path ..';%s/runtimes/'", path))
	{
		lemoonL_error(L, "set addition load path failed :%s", lua_tostring(L, -1));
	}

	
	if (LEMOON_RUNTIME_ERROR == lemoonL_dostring(L, "dofile(\"%s/runtimes/bootstrap.lua\")", path))
	{
		lemoonL_error(L, "set addition load path failed :%s", lua_tostring(L, -1));
	}

	return 0;
}

static void createargtable(lua_State *L, char **argv, int argc) {
	int i, narg;
	narg = argc - 1;  /* number of positive indices */
	lua_createtable(L, narg,0);
	for (i = 1; i < argc; i++) {
		lua_pushstring(L, argv[i]);
		lua_rawseti(L, -2, i);
	}
	lua_setglobal(L, "arg");
}


int lake_main(int args, char ** argv)
{
	int status = 0;

	lua_State * L = luaL_newstate();
	createargtable(L,argv,args);
	lua_pushcfunction(L, lake_pmain);

	if (0 != lua_pcall(L, 0, 0, 0))
	{
		printf("call pmain failed :%s\n", lua_tostring(L, -1));

		status = 1;
	}

	lua_close(L);

	return status;
}

int main(int args, char ** argv)
{
	return lake_main(args, argv);
}
