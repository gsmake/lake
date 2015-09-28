#include <lemoon/lfs.h>

#ifndef WIN32

#include <sys/stat.h>
#include <unistd.h>

static int lfs_exists(lua_State *L);

static int lfs_isdir(lua_State *L);

static int lfs_mkdir(lua_State *L);

static int lfs_current_path(lua_State *L);

static int lfs_create_directory_symlink(lua_State *L);

static const luaL_Reg lfs_funcs[] = {
    {"exists", lfs_exists},
    {"isdir", lfs_isdir},
    {"mkdir", lfs_mkdir},
    {"current", lfs_current_path},
    {"create_directory_symlink", lfs_create_directory_symlink},
    {NULL, NULL}
};

LEMOON_PRIVATE EXTERN_C int lfs_new(lua_State *L) {

  lua_newtable(L);

  luaL_setfuncs(L, lfs_funcs, 0);

  return 1;
}

int lfs_exists(lua_State *L) {

    const char *filepath = luaL_checkstring(L, 1);
    
    struct stat info = {0};
    
    if (stat(filepath, &info) == 0) {
        
        lua_pushboolean(L, 1);
        
        return 1;
    }
    
    lua_pushboolean(L, 0);
    
    return 1;
}

static int lfs_isdir(lua_State *L) {
    const char *filepath = luaL_checkstring(L, 1);
    
    struct stat info = {0};
    
    if (stat(filepath, &info) == 0) {
        
        if(S_ISDIR(info.st_mode)) {
            
            lua_pushboolean(L, 1);
            
            
            return 1;
        }
        
    }
    
    lua_pushboolean(L, 0);
    
    return 0;
}

static int lfs_mkdir(lua_State *L) {
    const char * path = luaL_checkstring(L, 1);
    
    if(mkdir(path, ACCESSPERMS) != 0) {
        return lemoonL_sysmerror(L, errno, "create dir %s error",path);
    }
    
    return 0;
}

static int lfs_current_path(lua_State *L) {
    
    char *dir = getcwd(NULL,0);
    
    lua_pushstring(L, dir);
    
    free(dir);
    
    return 1;
}

static int lfs_create_directory_symlink(lua_State *L) {
    const char * target = luaL_checkstring(L, 1);
    const char * linkpath = luaL_checkstring(L, 2);
    
    if(0 != symlink(target,linkpath)){
        lemoonL_sysmerror(L,errno,"call system#symlink error");
    }
    
    return 0;
}

#endif // WIN32
