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

return module
