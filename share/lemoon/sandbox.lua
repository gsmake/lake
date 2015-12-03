local class = require "lemoon.class"

local module = {}

local function loadScript(script,env)
    local closure,err = loadfile(script,"bt",env)

    if err ~= nil then
        error(string.format("load plugin -- failed,%s",err))
    end

    return closure()
end

function module:run(script,sandbox,...)
    local factory = require(sandbox)

    local originPath = _ENV.package.path
    local originCPath = _ENV.package.cpath
    local originLoaded = class.clone(_ENV.package.loaded)

    local env = class.clone(_ENV)

    factory.ctor(env,...)

    assert(env.package == _ENV.package,string.format("sandbox(%s) modified variable(package) : not allow",sandbox))

    local ret = { pcall(loadScript,script,env) }

    _ENV.package.path = originPath
    _ENV.package.cpath = originCPath

    for k in pairs(_ENV.package.loaded) do
        if originLoaded[k] == nil then
            _ENV.package.loaded[k] = nil
        end
    end


    if not ret[1] then
        error("\n\t" .. ret[2])
    end

    return table.unpack(ret,2)
end


return module

