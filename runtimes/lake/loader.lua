local class = require "lemoon.class"


-- cached logger
local logger = class.new("lemoon.log","lake")

local module = {}


-- load lake module by module local fs path
function module:load(path)
    logger:I("loading package :%s",path)

    -- check loading DCG

    local checkerOfDCG = self.checkerOfDCG

    local checkPath = ""

    local package = class.new("lake.package",self,path)

    for _,v in pairs(checkerOfDCG) do
        if v.Name == package.Name or #checkPath ~= 0 then
            checkPath = checkPath .. string.format("\t%s import\n",v.Name)
        end
    end

    if #checkPath ~= 0 then
        error(string.format("circular package import :\n%s\t%s",checkPath,package.Name))
    end

    logger:I("loading package :%s -- success",path)

    return package
end

function module.ctor(lake)
    local obj =
    {
        lake            = lake;
        checkerOfDCG    = {};
    }

    return obj
end

return module
