local fs = require "lemoon.fs"
local filepath = require "lemoon.filepath"
local cached = require "gsmake.cached"
local md5 = require "lemoon.md5"

local module = {
    GSMAKE_HOME = filepath.clean(os.getenv("GSMAKE_HOME")),
}


module.config = dofile(module.GSMAKE_HOME .. "/config/config.lua")

module.USERSPACE_HOME = module.config.USERSPACE_HOME

if module.USERSPACE_HOME == nil then
    module.USERSPACE_HOME  = filepath.join(module.GSMAKE_HOME,"userspace")
end

module.CACHED_HOME = module.config.CACHED_HOME

if module.CACHED_HOME  == nil then
    module.CACHED_HOME  = filepath.join(module.GSMAKE_HOME,"cached")
end

module.GSMAKE_PATH =  filepath.clean(fs.current() .. "/.gsmake.lua")

if not fs.exists(module.GSMAKE_PATH) then
    module.GSMAKE_PATH = module.GSMAKE_HOME .. "/.gsmake.lua"
end

module.USERSPACE = filepath.join(module.USERSPACE_HOME,md5.sumhexa(module.GSMAKE_PATH))

cached:open(module)

return module
