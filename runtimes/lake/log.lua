local colors = require 'lake.ansicolor'

local module = {}

function module.I(fmt, ...)
    print(colors('-- %{white}' .. string.format(fmt,...) .. '%{reset}'))
end

function module.E(fmt, ...)
    print(colors('-- %{red}' .. string.format(fmt,...) .. '%{reset}'))
end

function module.D(fmt, ...)
    print(colors('-- %{green}' .. string.format(fmt,...) .. '%{reset}'))
end


return module
