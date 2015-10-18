local fs = require "lemoon.fs"
local loader = require "gsmake.loader"
local filepath = require "lemoon.filepath"

local module = {
    HOME=os.getenv("GSMAKE_HOME"),
    WORKSPACE=fs.current()
}

if not fs.exists(filepath.join(module.WORKSPACE,".gsmake.lua")) then
    module.WORKSPACE = module.HOME
end

return module
