local module = lemoon.sys()


module.SO_NAME = ".so"

if module.host() == "Win32" or module.host() == "Win64" then
    module.SO_NAME = ".dll"
end

return module