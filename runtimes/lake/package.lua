-- the gsmake package module
local fs = require "lemoon.fs"
local class = require "lemoon.class"
local filepath = require "lemoon.filepath"



-- cached logger
local logger = class.new("lemoon.log","lake")

-- load package config file (.gsmake.lua)
local function loadConfig(package,configPath)

    local env = class.clone(_ENV)

    -- register name function
    env.name = function(name)
        logger:T("package name      :%s",name)
        package.Name = name
    end

    -- register version function
    env.version = function(version)
        logger:T("package version   :%s",version)
        package.Version = version
    end

    -- register plugin
    env.plugin = function(name)
        local plugin = class.new("lake.plugin",package,name)
        package.Plugins[name] = plugin
        return plugin
    end

    local closure,err = loadfile(configPath,"bt",env)

    if err ~= nil then
        error(string.format("load gsmake config file error :\n\tfile: %s\n\terr: %s",configPath,err))
    end

    closure()

    for _,plugin in pairs(package.Plugins) do
        plugin:load()
    end

end

local module = {}
function module.ctor(loader,path)

    assert(fs.exists(path),"package path not exists")

    local obj =
    {
        lake        = loader.lake;
        Path        = path;
        Version     = loader.lake.Config.GSMAKE_DEFAULT_VERSION;
        Plugins     = {};
    }

    local configPath = filepath.join(path,obj.lake.Config.GSMAKE_FILE)

    if fs.exists(configPath) then
        logger:V("found module config file :%s",configPath)
        -- do load package config file
        loadConfig(obj,configPath)
    end



    return obj
end


return module