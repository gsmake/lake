local log = require "lemoon.log"
local fs = require "lemoon.fs"
local filepath = require "lemoon.filepath"
local sqlite3 = require "lemoon.sqlite3"

local module = {}


function module:open(gsmake,userspace)
    self.GSMAKE = gsmake
    self.USERSPACE = filepath.join(self.GSMAKE.GSMAKE_HOME,"userspace",userspace)
    self.METADATA = filepath.join(self.USERSPACE,"metadata.db")

    -- create userspace directory
    if not fs.exists(self.USERSPACE) then
        fs.mkdirall(self.USERSPACE)
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
          id		       INTEGER PRIMARY KEY,
          source	       TEXT,
          source_version   TEXT,
          target	       TEXT,
          target_version   TEXT
        );

        CREATE TABLE packages (
          id		INTEGER PRIMARY KEY,
          name	    TEXT,
          domain	TEXT,
          version   TEXT
        );
        ]])

        if err ~= nil then
            log.E("init metadata database error :%s",err)
        end
    end
end

function module:sync(url)
    
end

return module
