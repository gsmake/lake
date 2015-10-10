local module = {}


function module.make(obj,funcs)
    local meta = {
        __index = funcs,
    }

    setmetatable(obj,meta)

    if funcs.ctor ~= nil then
        funcs.ctor(obj)
    end

    return obj
end

return module
