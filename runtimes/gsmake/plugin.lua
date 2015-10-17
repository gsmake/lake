local class = require "lemoon.class"
local log = require "lemoon.log"
local cached = require "gsmake.cached"
local config = require "gsmake".config
local urlparser  = require "lemoon.url"

local plugin = {}

function plugin:version(version)
    self.__version = version
end

function plugin:scm(scm)
    self.__scm = scm
end

function plugin:load(name)

    for _,v in pairs(config.sites) do
        local ok, scm, urlstring = v(name)

        if ok then
            local url = urlparser.parse(urlstring)

            url:setQuery({
                version = self.__version
            })

            local path = cached:sync(url,"plugin",scm)

            log.I("plugin local path :%s",path)

            return
        end
    end
end

local module = {}

function module.new()
    return class.make({
        __version = config.SCM.default_version
    },plugin)
end

return module
