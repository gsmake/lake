#ifndef LEMOON_LOG_LOG_HPP
#define LEMOON_LOG_LOG_HPP

#include <lua/lua.hpp>

namespace lemoon { namespace log{

    int luaopen_log(lua_State *L);
}}

#endif //LEMOON_LOG_LOG_HPP