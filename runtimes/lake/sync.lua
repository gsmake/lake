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

    logger:I("sync package <%s> ... ",package.Name)

    for _,remote in pairs(remotes) do

        local url = regex.gsub(package.Name,remote.Pattern,remote.URL)

        if url ~= package.Name then

            logger:I("sync(%s) %s",remote.Sync,url)

            local executorName = string.format("lake.sync.%s",remote.Sync)

            local ok, executor = pcall(class.new,executorName,self.lake,package)

            if not ok then
                error(string.format("load sync executor %s err :\n\t%s",executorName,executor))
            end

            executor:sync_remote(url)


            logger:I("sync(%s) %s -- success",remote.Sync,url)

            return
        end
    end

    logger:I("sync package <%s> -- failed, unsupport remote site",package.Name)
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

