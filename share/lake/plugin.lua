local class = require "lemoon.class"
local filepath = require "lemoon.filepath"

-- cached logger
local logger = class.new("lemoon.log","lake")



local module = {}
-- set plugin version
function module:version(version)
    self.Version = version
end

-- load plugin
function module:load()

    self.path = self.sync:sync(self)

    self:loadByPath(self.path)
end

function loadMain(mainPath,env)
    local closure,err = loadfile(mainPath,"bt",env)

    if err ~= nil then
        error(string.format("load plugin -- failed,%s",err))
    end

    closure()
end

function module:loadByPath(path)

    logger:D("load plugin\n\tname :%s\n\tversion :%s\n\tpath :%s",self.Name,self.Version,path)

    local mainPath = filepath.join(path,"plugin.lua")

    local sandbox = class.new("lemoon.sandbox")

    sandbox:run(mainPath,"lake.sandbox.plugin",self,path)

    for _,task in pairs(self.Tasks) do

        self.lake.Runner:add(task)
    end

end

-- plugin constructor
function module.ctor(package,name)
    -- init plugin object
    local obj = {

        lake        = package.lake;

        sync        = package.lake.Sync;

        Package     = package;

        Name        = name;

        Version     = package.lake.Config.GSMAKE_DEFAULT_VERSION;

        Tasks       = {}                                        ; -- package scope register tasks
    }

    return obj
end

return module

