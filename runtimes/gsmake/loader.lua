local class = require "lemoon.class"
local log = require "lemoon.log"
local plugin = require "gsmake.plugin"


local loader = {}

function loader:ctor()
    self.prepare_plugins = {}
    self.plugins = {}
end

function loader:plugin(name)
    local obj = plugin.new()
    self.prepare_plugins[name] = obj
    return obj
end

function loader:load_plugins()
    for k,v in pairs(self.prepare_plugins) do
        v:load(k)
    end
end

function loader:__index(key)

    self:load_plugins()

    for k,v in pairs(loader) do
        if k == key then
            return v
        end
    end

end


local module = {}

function module.load(path)

    -- TODO: check if the .gsmake.lua file exists

    local env = {}

    for k,v in pairs(_ENV) do
        env[k] = v
    end

    env.publish = function(name)
       env.loader = class.make({ name = name },loader)
       return env.loader
    end

    local metadata = {}

    metadata.__index = function (table, key)

      local v = env.loader:__index(key)

      if type(v) == "function" then
          return function(...)
              return v(env.loader,...)
          end
      end

      return v

    end

    setmetatable(env,metadata)

    local call,err = loadfile(path,"bt",env)

    if err ~= nil then
        error(err)
    end

    -- call .gsmake.lua script
    call()
end

return module
