local fs        = require "lemoon.fs"
local class     = require "lemoon.class"
local filepath  = require "lemoon.filepath"


-- cached logger
local logger = class.new("lemoon.log","lake")

local module = {}
function module.ctor(name,owner)

    local obj = {
        Name        = name                                         ; -- plugin name
        Version     = owner.lake.Config.GSMAKE_DEFAULT_VERSION     ; -- plugin version
        Owner       = owner                                        ; -- the package plugin belongs to
        sync        = owner.lake.Sync                              ; -- gsmake sync service
        loader      = owner.lake.Loader                            ; -- gsmake context
        install_dir = owner.lake.Config.GSMAKE_INSTALL_PATH        ; -- plugin install path
    }

    return obj
end

function module:version(version)
    self.Version = version
end

function module:load()
    -- first sync the plugin's package
    logger:D("sync plugin package [%s:%s]",self.Name,self.Version)
    local sourcePath = self.sync:sync(self.Name,self.Version)
    logger:D("sync plugin package [%s:%s] -- success\n\tpath :%s",self.Name,self.Version,sourcePath)

    self.Package = self.loader:load(sourcePath,self.Name,self.Version)

    return self.Package

end

function module:setup()

    self.Path = filepath.join(self.install_dir,"gsmake","plugin")

    if not fs.exists(self.Path) then
        error(string.format("[%s:%s]'s plugin [%s:%s] not install ",self.Owner.Name,self.Owner.Version,self.Name,self.Version))
    end

    local pluginMain = filepath.join(self.Path,"plugin.lua")

    local sandbox = class.new("lemoon.sandbox")

    sandbox:run(pluginMain,"lake.sandbox.plugin",self,path)

end

return module
