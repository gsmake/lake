--
-- this file is gsmake boostrap lua script file
--
local sys   = require "lemoon.sys"
local class = require "lemoon.class"

local lake = class.new("lake",{
    GSMAKE_FILE                 = ".gsmake.lua";
    GSMAKE_CACHED_DIR           = ".gsmake";
    GSMAKE_ENV                  = "GSMAKE_HOME";
    GSMAKE_DEFAULT_VERSION      = "snapshot";
    GSMAKE_TARGET_HOST          = sys.host();
})

lake:run()





