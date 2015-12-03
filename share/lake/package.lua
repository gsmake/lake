-- the gsmake package module
local fs = require "lemoon.fs"
local class = require "lemoon.class"
local filepath = require "lemoon.filepath"



-- cached logger
local logger = class.new("lemoon.log","lake")


local module = {}

function module:load()
    for _,plugin in pairs(self.Plugins) do
        plugin:load()
    end

    for _,task in pairs(self.Tasks) do
        self.lake.Runner:add(task)
    end
end

function module.ctor(loader,path,name,version)

    assert(fs.exists(path),"package path not exists")

    local obj =
    {
        lake        = loader.lake                               ;
        Path        = path                                      ;
        Version     = loader.lake.Config.GSMAKE_DEFAULT_VERSION ;
        Plugins     = {}                                        ; -- package scope register plugins
        Tasks       = {}                                        ; -- package scope register tasks
        Properties  = {}                                        ; -- package scrope properties
    }

    local configPath = filepath.join(path,obj.lake.Config.GSMAKE_FILE)

    if fs.exists(configPath) then
        logger:V("found module config file :%s",configPath)
        -- do load package config file

        local sandbox = class.new("lemoon.sandbox")

        sandbox:run(configPath,"lake.sandbox.package",obj)
    end

    if name ~= nil then
        obj.Name = name
    end

    if version ~= nil then
        obj.Version = version
    end


    return obj
end


return module