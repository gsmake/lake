local log = require "lake.log"
local lake = require "lake.lake"

local module = {}

function module:version(version)
    self.version = version
end

function module:load(url)

    for k,v in pairs(lake:load(url) or error("\n\tcan't apply \"" .. url .. "\" as plugin : .gsmake.lua not provide")) do
        self[k] = v
    end
end

return require "lake.class" .create(module)
