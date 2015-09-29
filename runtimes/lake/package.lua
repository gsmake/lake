-- package module
local module = {}

function module.load(path)

    local env = {}

    for k,v in pairs(_ENV) do
        env[k] = v
    end

    local lake = require "lake.sandbox" .new({})

    env.publish = function(name)
        env.__lake = require "lake.sandbox" .new({ name = name })
        return env.__lake
    end

    local metadata = {}

    metadata.__index = function (table, key)

      local v = env.__lake:__index(key)

      if type(v) == "function" then
          return function(...)
              return v(env.__lake,...)
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

    return env.__lake
end

return module
