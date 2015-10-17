local log = require "lemoon.log"
local gsmake = require "gsmake"
local urlparser = require "lemoon.url"
local filepath = require "lemoon.filepath"
local fs = require "lemoon.fs"
local exec = require "lemoon.exec"

local module = {}

function module.sync(cached,urlstring,stage)
    local url = urlparser.parse(urlstring)
    local version = url.query.version

    if version == gsmake.config.SCM.default_version then
        version = "master"
    end

    url:setQuery({})

    local urlstring = tostring(url)

    local ok,cachedpath = cached:queryC0(urlstring)

    if not ok then

        cachedpath = filepath.join(gsmake.CACHED_HOME,"git",url.host,url.path)

        if fs.exists(cachedpath) then
            fs.removeall(cachedpath)
        end

        exec.run(string.format("git clone --mirror %s %s",urlstring,cachedpath))

        cached:setC0(urlstring,cachedpath)
    end

    local ok,userspace = cached:queryC1(stage,urlstring)

    if not ok then

        userspace = filepath.join(gsmake.USERSPACE,stage,"src",url.host,url.path)
        if fs.exists(userspace) then
            fs.removeall(userspace)
        end

        exec.run(string.format("git clone %s %s",cachedpath,userspace))

        cached:setC1(stage,urlstring,userspace)
    end

    exec.run(string.format("git checkout %s",version),userspace)

    log.I("[git] sync remote repo    : %s",urlstring)
    log.I("[git] sync version        : %s",version)
    log.I("[git] sync cached path    : %s",cachedpath)
    log.I("[git] sync userspace path : %s",userspace)

    return userspace

end

return module
