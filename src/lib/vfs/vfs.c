#include <vfs/vfs.h>
#include <sqlite/sqlite3.h>


#define VFS_CLASS "__vfs_class"

static int vfs_open(lua_State *L);


static luaL_Reg vfs_funcs[] = {
    {"open",vfs_open},
    {NULL, NULL}
};



LEMOON_API int luaopen_vfs(lua_State*L){
    luaL_newlib(L, vfs_funcs);
    return 1;
}

typedef struct vfs_t{
    
    sqlite3     *db; //sqlite open handler
    
} vfs_t;

static luaL_Reg vfs_class[] = {
    {NULL, NULL}
};

static int vfs_class_close(lua_State*L) {
    
    vfs_t * self = (vfs_t*)luaL_checkudata(L, 1, VFS_CLASS);
    
    sqlite3_close(self->db);
    
    return 0;
}


static int vfs_open(lua_State *L){
    
    const char * name = luaL_checkstring(L, 1);

    
    vfs_t * vfs = lemoon_newclass(L,VFS_CLASS,sizeof(vfs_class),vfs_class,vfs_class_close);
    
    int errcode = sqlite3_open(name,&vfs->db);
    
    if(SQLITE_OK != errcode){
        lemoonL_error(L,"open sqlite3 for %s error",name,sqlite3_errstr(errcode));
    }
    
    return 1;
}