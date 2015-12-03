local class = require "lemoon.class"


-- cached logger
local logger = class.new("lemoon.log","lake")


local module = {}
function module.ctor(lake)
    local obj = {
        lake = lake;
        group = {};
    }

    return obj
end


function module:add(task)

    logger:D("add task <%s:%s> ;%s",task.Package.Name,task.Name,task.Description)

    if self.group[task.Name] == nil then
        self.group[task.Name] = {task}
    else
        table.insert(self.group[task.Name],task)
    end
end


return module

