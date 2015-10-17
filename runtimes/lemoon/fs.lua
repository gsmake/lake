local module = lemoon.fs()
local filepath = require "lemoon.filepath"

function module.mkdirall(path)
    local nodes = {}

    while true do
        nodes[#nodes+1] = path

        local base = filepath.dir(path)

        if path == base then
            break
        end

        path = base
    end

    for i = #nodes,1,-1 do
        if not module.exists(nodes[i]) then
            module.mkdir(nodes[i])
        end
    end
end

function module.removeall(path)
    if not module.isdir(path) then
        if module.exists(path) then
            module.rmfile(path)
            return
        end
    end

    module.ls(path,function(entry)
        if entry ~= '.' and entry ~= ".." then
            module.removeall(filepath.join(path,entry))
        end
    end)

    module.rmdir(path)
end

return module
