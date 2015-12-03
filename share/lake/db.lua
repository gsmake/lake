local class = require "lemoon.class"
local sqlite3 = require "lemoon.sqlite3"
local filepath = require "lemoon.filepath"

-- cache the logger object
local logger = class.new("lemoon.log","lake")


local module = {}

local sqlexec = function(db,sql)
    local id, err = db:exec(sql)

    if err ~= nil then
        error(err)
    end

    return id
end

function module.ctor(lake)

    local obj = {
        lake = lake;
    }

    logger:T("open gsmake database ...")

    obj.localdb = assert(sqlite3.open(filepath.join(lake.CachedDir,"gsmake.db")))

    sqlexec(obj.localdb, [[
        create table if not exists _INSTALL
        (
           _NAME        TEXT,
           _PATH        TEXT,
           _SOURCE      TEXT,
           _VERSION     TEXT,
           _HOST        TEXT
        );

        create unique index if not exists _INSTALL_FULLNAME_INDEXER ON _INSTALL (_NAME,_VERSION,_HOST);
    ]])

    obj.globaldb = assert(sqlite3.open(filepath.join(lake.GSMAKE_HOME_REPO,"repo.db")))

    sqlexec(obj.globaldb, [[

        create table if not exists _SOURCE
        (
           _NAME        TEXT,
           _PATH        TEXT,
           _SOURCE      TEXT,
           _VERSION     TEXT
        );

        create unique index if not exists _SOURCE_FULLNAME_INDEXER ON _SOURCE (_NAME,_VERSION);

        create table if not exists _SYNC
        (
           _NAME        TEXT,
           _PATH        TEXT,
           _SOURCE      TEXT,
           _VERSION     TEXT,
           _SYNC        TEXT
        );

        create unique index if not exists _SYNC_FULLNAME_INDEXER ON _SYNC (_NAME,_VERSION,_SYNC);
    ]])

    logger:T("open gsmake database -- success")

    return obj
end

-- query install package path
function module:query_install(name,version)

    local SQL = string.format('SELECT * FROM _INSTALL WHERE _NAME="%s" and _VERSION="%s" and _HOST="%s"',name,version,self.lake.GSMAKE_TARGET_HOST)

    for _,path,_,_ in self.localdb:urows(SQL) do
        return path,true
    end

    return "",false
end

function module:query_source(name,version)
    local SQL = string.format('SELECT * FROM _SOURCE WHERE _NAME="%s" and _VERSION="%s"',name,version)

    for _,path,_,_ in self.globaldb:urows(SQL) do
        return path,true
    end

    return "",false
end

function module:query_sync(name,version)
    local SQL = string.format('SELECT * FROM _SYNC WHERE _NAME="%s" and _VERSION="%s"',name,version)

    for _,path,_,_,_ in self.globaldb:urows(SQL) do
        return path,true
    end

    return "",false
end

function module:save_install(name,version,source,path)

    local SQL = string.format('insert into _INSTALL VALUES("%s","%s","%s","%s","%s")',name,path,source,version,self.lake.GSMAKE_TARGET_HOST)

    sqlexec(self.localdb,SQL)

end

function module:save_sync(name,version,source,path,sync)

    local SQL = string.format('insert into _SYNC VALUES("%s","%s","%s","%s","%s")',name,path,source,version,sync)

    sqlexec(self.globaldb,SQL)

end

function module:save_source(name,version,source,path)

    local SQL = string.format('insert into _SOURCE VALUES("%s","%s","%s","%s")',name,path,source,version)

    sqlexec(self.globaldb,SQL)

end

return module
