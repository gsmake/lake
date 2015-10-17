local log = require "lemoon.log"
local fs = require "lemoon.fs"
local filepath = require "lemoon.filepath"
local sqlite3 = require "lemoon.sqlite3"
local urlparser = require "lemoon.url"

local module = {}


function module:open(gsmake,userspace)
    self.GSMAKE = gsmake
    self.USERSPACE = gsmake.USERSPACE
    self.METADATA = filepath.join(gsmake.GSMAKE_HOME,".db/metadata.db")
    self.DB_HOME = filepath.dir(self.METADATA)

    -- create userspace directory
    if not fs.exists(self.DB_HOME) then
        fs.mkdirall(self.DB_HOME)
    end

    -- init metadata table
    local initflag = false
    if not fs.exists(self.METADATA) then
        initflag = true
    end

    self.metadata_db = assert(sqlite3.open(self.METADATA))

    if initflag then
        local _,err = self.metadata_db:exec([[
        CREATE TABLE redirect (
          id		       INTEGER PRIMARY KEY AUTOINCREMENT ,
          source	       TEXT,
          source_scm       TEXT,
          target	       TEXT,
          target_scm       TEXT
        );

        CREATE TABLE cached (
          id		INTEGER PRIMARY KEY AUTOINCREMENT ,
          url	    TEXT,
          local     TEXT
        );

        CREATE TABLE userspace (
          id		INTEGER PRIMARY KEY AUTOINCREMENT,
          userspace	TEXT,
          stage	    TEXT,
          url       TEXT,
          path      TEXT
        );
        ]])

        if err ~= nil then
            log.E("init metadata database error :%s",err)
        end
    end
end

function module:sync(url,stage,scm)
    log.I("[%s] sync remote repo[%s] :%s",stage,scm,tostring(url))

    local redirectSQL = string.format("SELECT target,target_scm FROM redirect where source = '%s' and source_scm = '%s'",tostring(url),scm)

    for row in self.metadata_db:nrows(redirectSQL) do
        url = urlparser.parse(row.target)
        scm = row.target_scm
        log.I("[%s] redirect -> remote repo[%s] :%s",stage,scm,tostring(url))
    end

    local sync = require("gsmake.scm."..scm)

    return sync.sync(self,tostring(url),stage)
end

function module:queryC0(url)
    -- check cached repo

    local cachedQuery = string.format("SELECT local FROM cached where url = '%s'",url)

    for row in self.metadata_db:nrows(cachedQuery) do
        return true,row["local"]
    end

    return false
end

function module:setC0(url,cachedpath)
    local _,err = self.metadata_db:exec(string.format("INSERT INTO cached(url,local) VALUES ('%s', '%s')",url,cachedpath))

    if err ~= nil then
        error(string.format("write C0 metadata error :%s",err))
    end
end

function module:queryC1(stage,url)

    local cachedQuery = string.format("SELECT path FROM userspace where userspace = '%s' and stage = '%s' and url = '%s'",self.USERSPACE,stage,url)

    for row in self.metadata_db:nrows(cachedQuery) do
        return true,row.path
    end

    return false
end

function module:setC1(stage,url,cachedpath)
    local _,err = self.metadata_db:exec(string.format("INSERT INTO userspace(userspace,stage,url,path) VALUES ('%s','%s','%s','%s')",self.USERSPACE,stage,url,cachedpath))

    if err ~= nil then
        error(string.format("write C1 metadata error :%s",err))
    end
end

return module
