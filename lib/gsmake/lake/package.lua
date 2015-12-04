-- the gsmake package module
local fs        = require "lemoon.fs"
local sys       = require "lemoon.sys"
local class     = require "lemoon.class"
local filepath  = require "lemoon.filepath"

-- cached logger
local logger = class.new("lemoon.log","lake")

local module = {}

function module.ctor(lake,path,name,version)
    assert(fs.exists(path),"package path not exists")

    local obj =
    {
        lake        = lake                                           ;
        Name        = name                                           ;
        Version     = version or  lake.Config.GSMAKE_DEFAULT_VERSION ;
        Path        = path                                           ;
        Plugins     = {}                                             ; -- package scope register plugins
        Tasks       = {}                                             ; -- package scope register tasks
        Properties  = {}                                             ; -- package scrope properties
    }

    local gsmakeFilePath = filepath.join(path,lake.Config.GSMAKE_FILE)

    if fs.exists(gsmakeFilePath) then
        logger:V("found a standard lake package :%s",path)

        local sandbox = class.new("lemoon.sandbox")

        sandbox:run(gsmakeFilePath,"lake.sandbox.package",obj)
    end

    return obj
end

function module:link()

    logger:I("load package [%s:%s] ... ",self.Name,self.Version)

    for _,plugin in pairs(self.Plugins) do

        -- install dependency plugin
        logger:D("[%s:%s] load and link plugin [%s:%s]",self.Name,self.Version,plugin.Name,plugin.Version)

        local package = plugin:load()

        package:link()

        logger:D("[%s:%s] load and link plugin [%s:%s] -- success",self.Name,self.Version,plugin.Name,plugin.Version)
    end


    logger:I("load package [%s:%s] -- success ",self.Name,self.Version)
end

function module:setup()

    local gsmakePath = filepath.join(self.lake.Config.GSMAKE_HOME,"bin","gsmake",sys.EXE_NAME)

    for _,plugin in pairs(self.Plugins) do

        logger:D("[%s:%s] link plugin [%s:%s]",self.Name,self.Version,plugin.Name,plugin.Version)

        local package = plugin.Package

        local exec = sys.exec(gsmakePath)

        exec:dir(package.Path)

        exec:start("install",self.lake.Config.GSMAKE_INSTALL_PATH)

        if exec:wait() ~= 0 then
            error(string.format("[%s:%s] install plugin [%s:%s] -- failed",self.Name,self.Version,plugin.Name,plugin.Version))
        end

        logger:D("[%s:%s] link plugin [%s:%s] -- success",self.Name,self.Version,plugin.Name,plugin.Version)

        logger:D("[%s:%s] setup plugin [%s:%s]",self.Name,self.Version,plugin.Name,plugin.Version)

        plugin:setup()

        logger:D("[%s:%s] setup plugin [%s:%s] -- success",self.Name,self.Version,plugin.Name,plugin.Version)
    end
end


return module
