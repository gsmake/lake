#ifndef LEMOON_REGEX_HPP
#define LEMOON_REGEX_HPP

#include <lua/lua.hpp>

namespace lemoon { namespace regex{

    int luaopen_regex(lua_State *L);
}}

#endif //LEMOON_REGEX_HPP