-- package preinstall loader

local fs = require "lemoon.fs"
local log = require "lemoon.log"
local class = require "lemoon.class"
local filepath = require "lemoon.filepath"
local plugin = require "gsmake.plugin"


local module = {}

function module:getStage(name)
    if self.plugin ~= nil then
        for _,plugin in pairs(self.plugin) do
            plugin:sync()
            plugin:load(self)
        end
    end

    return self.stage[name]
end

-- load package by filesystem path
function module.load(path)

    local loader = class.make({
        plugin = {},
        stage = {}
    },module)

    local metafile = filepath.join(path,".gsmake.lua")

    -- check if this package is external package
    if not fs.exists(metafile) then
        return loader,false
    end

    -- create package's env
    local env = class.clone(_ENV)

    env.name = function(name)
        loader.packagename = name
    end

    env.plugin = function(name)
        loader.plugin[name] = plugin.make(name)
        return loader.plugin[name]
    end

    local metatable = {
        __index = function(self,name)
            return loader:getStage(name)
        end
    }

    setmetatable(env,metatable)

    local call,err = loadfile(metafile,"bt",env)

    if err ~= nil then
        error(err)
    end

    -- dofile
    call()

    return loader
end

return module
