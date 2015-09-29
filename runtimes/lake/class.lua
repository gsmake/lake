local module = {}

function module.create(prototype)
    if type(prototype) ~= "table" then
        error("prototype must be a table")
    end

    local class = {}

    local metatable = { __index = prototype }

    class.new = function(obj)

        if type(obj) ~= "table" then
            error("\n\tclass new must provide table context\n" .. debug.traceback())
        end

        setmetatable(obj,metatable)

        local ctor = obj.ctor

        if ctor ~= nil then
            ctor(obj)
        end

        return obj
    end

    return class

end

return module
