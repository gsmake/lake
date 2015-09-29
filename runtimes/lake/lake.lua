local log = require "lake.log"
local fs = require "lake.fs"

-- loading package stack
local loadingstack = require "lake.list" .new()

local loaded = {}


local module = {
    HOME_PATH = os.getenv("LAKE_HOME"),
}


local function doload(path)
    log.D("loading package :" .. path)

    local lakefile = path .. "/.gsmake.lua"

    if not fs.exists(lakefile) then
        log.I("skip load package : .gsmake.lua not found\n\tpath :" .. path)
        return 
    end

    local metadata = require "lake.package" .load(lakefile)

    loaded[metadata.name] = metadata

    return metadata
end

function module:load(uri)
    if uri == nil then
        return doload(self.WORKSPACE)
    else
        return doload(uri)
    end
end

-- query loaded package
function module:loaded(name)
    return loaded[name]
end

return module
