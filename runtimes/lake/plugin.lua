local class = require "lake.class"

local module = {}

function module:version(version)
    self.version = version
end

return class.create(module)
