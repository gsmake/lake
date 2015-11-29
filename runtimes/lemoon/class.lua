local module = {}

function module.new(name,...)

    local metatable = require(name)

    local obj = nil

    if metatable.ctor ~= nil then

        obj = metatable.ctor(...)
    else
        obj = {...}
    end

    setmetatable(obj,{
         __index = metatable;
         __gc =metatable.final;
     })

    return obj
end


return module
