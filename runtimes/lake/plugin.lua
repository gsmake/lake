local class = require "lemoon.class"

-- cached logger
local logger = class.new("lemoon.log","lake")



local module = {}
-- set plugin version
function module:version(version)
    self.Version = version
end

-- load plugin
function module:load()
    logger:I("load plugin <%s:%s>",self.Name,self.Version)
    self.sync:sync(self)
end

-- plugin constructor
function module.ctor(package,name)
    -- init plugin object
    local obj = {

        sync    = package.lake.Sync;

        Package     = package;

        Name        = name;

        Version     = package.lake.Config.GSMAKE_DEFAULT_VERSION;
    }

    return obj
end

return module

