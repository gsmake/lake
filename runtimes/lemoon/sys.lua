local capi = lemoon.sysinfo()
local module = {}

function module.iswindows()
    if capi.osname() == "win32" or capi.osname() == "win64" then
        return true
    end

    return false
end

return module
