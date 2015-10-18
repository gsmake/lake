local class = require "lemoon.class"

local module = {}


function module:version(version)
    self.versionstring = version
end

function module:sync()

end

function module:load(loader)

end

function module.make(name)
    return class.make({name=name},module)
end

return module
