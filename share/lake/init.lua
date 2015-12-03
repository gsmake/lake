local fs = require "lemoon.fs"
local class = require "lemoon.class"
local filepath = require "lemoon.filepath"

-- cached logger
local logger = class.new("lemoon.log","lake")

local module = {}

function module.ctor(config)
    logger:I("start lake ...")

    -- check the .gsmake.lua file
    local workspace = fs.dir()
    if not fs.exists(fs.dir() .. "/" .. config.GSMAKE_FILE) then
        workspace = os.getenv(config.GSMAKE_ENV)
    end

    local lake = {
        Config                  = config;
        GSMAKE_HOME             = os.getenv(config.GSMAKE_ENV);
        GSMAKE_HOME_REPO        = filepath.join(os.getenv(config.GSMAKE_ENV),"repo");
        GSMAKE_TARGET_HOST      = config.GSMAKE_TARGET_HOST;
        Workspace               = workspace;
        CachedDir               = filepath.join(workspace,config.GSMAKE_CACHED_DIR);
    }

    logger:D("workspace     :%s",workspace)
    logger:D("target name   :%s",lake.GSMAKE_TARGET_HOST)

    if fs.exists(lake.CachedDir) and not fs.isdir(lake.CachedDir) then
        fs.rm(lake.CachedDir)
    end

    if not fs.exists(lake.CachedDir) then
        fs.mkdir(lake.CachedDir)
    end

    if fs.exists(lake.GSMAKE_HOME_REPO) and not fs.isdir(lake.GSMAKE_HOME_REPO) then
        fs.rm(lake.GSMAKE_HOME_REPO)
    end

    if not fs.exists(lake.GSMAKE_HOME_REPO) then
        fs.mkdir(lake.GSMAKE_HOME_REPO)
    end

    lake.Database  = class.new("lake.db",lake)      -- create new database
    lake.Sync      = class.new("lake.sync",lake)    -- create new sync service
    lake.Runner    = class.new("lake.runner",lake);

    logger:I("start lake -- success")

    return lake
end

function module:run()
    self.loader = class.new("lake.loader",self)

    -- set bootstrap plugin

    local _,ok = self.Database:query_install("github.com/gsmake/clang","bootstrap")

    if not ok then
        local clangPath = filepath.join(self.GSMAKE_HOME,"share","lake","plugin","clang")

        self.Database:save_install("github.com/gsmake/clang","bootstrap",clangPath,clangPath)
    end

    local _,ok = self.Database:query_install("github.com/gsmake/lua","bootstrap")

    if not ok then
        local luaPath = filepath.join(self.GSMAKE_HOME,"share","lake","plugin","lua")

        self.Database:save_install("github.com/gsmake/lua","bootstrap",luaPath,luaPath)
    end

    self.loader:load(self.Workspace)
end

return module
