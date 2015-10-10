local class = require "lemoon.class"
local log = require "lemoon.log"
local cached = require "gsmake.cached"
local url  = require "lemoon.url"
local config = require "gsmake".config

local plugin = {}

function plugin:version(version)
    self.version = version
end

function plugin:scm(scm)
    self.scm = scm
end

function plugin:load(u)
    for k,v in pairs(config.sites) do
        log.I(k)
    end
end

local module = {}

function module.new()
    return class.make({
        scm = config.SCM.default
    },plugin)
end

return module
