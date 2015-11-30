-- the lake package sync services
local fs = require "lemoon.fs"
local regex = require "lemoon.regex"
local class = require "lemoon.class"
local remotes = require "lake.remotes"

-- cached logger
local logger = class.new("lemoon.log","lake")

local module = {}

function module:install(package)
    logger:I("install")
end

function module:sync_cached(package)
    logger:I("sync_cached")
end

function module:sync_remote(package)

    logger:I("sync remote :%s",package.Name)

    for name,remote in pairs(remotes) do

        logger:V("%s",remote.Pattern)

        local url = regex.gsub(package.Name,remote.Pattern,remote.URL)

        logger:V("replace :%s",url)

        if url ~= package.Name then
            logger:I("wellknown remote :%s",name)
        end
    end

    logger:I("sync_remote")
end


function module:sync(package)

    local path,ok = self.db:query_install(package.Name,package.Version)

    if ok and fs.exists(path) then
        return path
    end

    path,ok = self.db:query_source(package.Name,package.Version)

    if ok and fs.exists(path) then
        return self:install(package.Name,package.Version)
    end

    path,ok = self.db:query_sync(package.Name,package.Version)

    if ok and fs.exists(path) then
        self:sync_cached(package)
        return self:install(package)
    end

    self:sync_remote(package)
    self:sync_cached(package)
    return self:install(package)

end

function module.ctor(lake)
    local obj = {
        lake        = lake;
        db          = lake.Database;
    }

    return obj
end

return module

