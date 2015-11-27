#ifndef LEMOON_FS_FS_HPP
#define LEMOON_FS_FS_HPP

#include <lua/lua.hpp>

namespace lemoon { namespace fs{

    int luaopen_fs(lua_State *L);
}}

#endif //LEMOON_FS_FS_HPP