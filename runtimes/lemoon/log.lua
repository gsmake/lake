local colors = require 'lemoon.ansicolor'

local module = {
    VERBOSE = 0, DEBUG = 1, INFO = 2, WARN = 4, ERROR = 8
}

local __level = module.VERBOSE | module.DEBUG | module.INFO | module.WARN | module.ERROR

function module.Flags(flags)
    __level = flags
end


function module.I(fmt, ...)
    print(colors('%{white}' .. string.format(fmt,...) .. '%{reset}'))
end

function module.E(fmt, ...)
    print(colors('%{red}' .. string.format(fmt,...) .. '%{reset}'))
end

function module.D(fmt, ...)
    print(colors('%{green}' .. string.format(fmt,...) .. '%{reset}'))
end

return module
