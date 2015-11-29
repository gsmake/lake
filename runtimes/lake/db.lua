local fs = require "lemoon.fs"
local class = require "lemoon.class"
local sqlite3 = require "lemoon.sqlite3"

-- cache the logger object
local logger = class.new("lemoon.log","lake")


local module = {}

function module.ctor(lake)

    local obj = {lake = lake}

    obj.sqlite3 = assert(sqlite3.open(lake.CachedDir .. "/gsmake.db"))

    local _, err = obj.sqlite3:exec [[
        create table if not exists test(name) ;
    ]]

    if err ~= nil then
        error(err)
    end

    return obj
end

return module
