local class = require "lemoon.class"


-- cached logger
local logger = class.new("lemoon.log","lake")

local module = {}


function module.ctor(lake)

    local obj =
    {
        lake            = lake;
        checkerOfDCG    = {};
        packages        = {};
    }

    return obj
end

function module:load(path,name,version,metadata)


    local package = class.new("lake.package",self.lake,path,name,version)

    if not metadata then package:load() end

    return package
end

return module

