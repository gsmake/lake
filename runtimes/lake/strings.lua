local module = {}


function module.printenum(val)

    if type(val) ~= "table" then
        return tostring(val)
    end

    local txt = "{"

    for _,v in ipairs(val) do
        txt = txt .. v .. ", "
    end

    txt = txt .. '}'

    return txt:gsub(", }","}")
end

return module
