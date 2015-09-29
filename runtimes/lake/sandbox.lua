local plugin  = require "lake.plugin"
local log  = require "lake.log"
local lake  = require "lake.lake"

local module = {}

function module:ctor()
    self.prepare_plugins = {}
    self.plugins = {}
end

function module:plugin(name)
    local obj = plugin.new({})
    self.prepare_plugins[name] = obj
    return obj
end

function module:load_plugins()

    for k,v in pairs(self.prepare_plugins) do
        log.D("loading plugin :%s",k)

        log.D("loading plugin :%s -- success",k)
    end
end

function module:__index(key)

    for k,v in pairs(module) do
        if k == key then
            return v
        end
    end

    self:load_plugins()

    return self.plugins[key]
end

return require "lake.class" .create(module)
