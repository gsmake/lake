local fs = require "lemoon.fs"

local module = {}

function module.run(command,workdir)
    local current = fs.current()
    if workdir ~= nil then
        fs.setcurrent(workdir)
    end

    local ok,status,code = os.execute(command)

    fs.setcurrent(current)

    if not ok  then
        error(string.format("exec command %s %s code :%d",command,status,code))
    end
end

-- searches for an executable binary named file in the directories named
-- by the PATH environment
function module.search(execute)

end

return module
