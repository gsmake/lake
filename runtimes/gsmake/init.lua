local fs = require "lemoon.fs"
local filepath = require "lemoon.filepath"
local cached = require "gsmake.cached"
local md5 = require "lemoon.md5"

local module = {
    GSMAKE_HOME= filepath.clean(os.getenv("GSMAKE_HOME")),
}


module.config = dofile(module.GSMAKE_HOME .. "/config/config.lua")

module.GSMAKE_PATH =  filepath.clean(fs.current() .. "/.gsmake.lua")

if not fs.exists(module.GSMAKE_PATH) then
    module.GSMAKE_PATH = module.GSMAKE_HOME .. "/.gsmake.lua"
end

cached:open(module,md5.sumhexa(module.GSMAKE_PATH))

return module
