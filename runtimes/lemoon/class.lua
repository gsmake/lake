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

function module.clone(table)
    local clone = {}

    for k,v in pairs(table) do
       clone[k] = v
    end

    setmetatable(clone,getmetatable(table))

    return clone
end

return module
