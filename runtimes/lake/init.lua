local fs = require "lemoon.fs"
local sys = require "lemoon.os"
local class = require "lemoon.class"

-- cached logger
local logger = class.new("lemoon.log","lake")

local module = {}

function module.ctor()
    -- check the .gsmake.lua file
    local workspace = fs.dir()
    if not fs.exists(fs.dir() .. "/.gsmake.lua") then
        workspace = os.getenv("GSMAKE_HOME")
    end

    local lake = {
        Workspace  = workspace;
        CachedDir  = workspace .. "/.gsmake"
    }

    if fs.exists(lake.CachedDir) and not fs.isdir(lake.CachedDir) then
        fs.rm(lake.CachedDir)
    end

    if not fs.exists(lake.CachedDir) then
        fs.mkdir(lake.CachedDir)
    end

    lake.Database  = class.new("lake.db",lake); -- create new database

    logger:I("start lake ...")
    logger:D("workspace : %s",workspace)

    return lake
end

function module:run()
end

return module
