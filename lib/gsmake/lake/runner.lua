local class     = require "lemoon.class"

local logger    = class.new("lemoon.log","lake")

local module = {}

function module.ctor(package)
    local obj = {
        package         = package;

        taskGroups      = {};

        checkerOfDCG    = {};
    }

    return obj
end

function module:run(name,...)

    for _,plugin in pairs(self.package.Plugins or {}) do
        for name,task in pairs(plugin.Tasks or {}) do

            if self.taskGroups[name] == nil then
                self.taskGroups[name] = { Name = name,task }
            else
                table.insert(self.taskGroups[name],task)
            end
        end
    end

    for name,task in pairs(self.package.Tasks or {}) do

        if self.taskGroups[name] == nil then
            self.taskGroups[name] = {Name = name, task}
        else
            table.insert(self.taskGroups[name],task)
        end
    end

    for name,taskgroup in pairs(self.taskGroups) do
        logger:D("register taskgroup(%s) :",name)
        for _,task in ipairs(taskgroup or {}) do
            logger:D("\tfrom package [%s:%s]",task.Package.Name,task.Package.Version)
        end
    end

    local taskGroup = self.taskGroups[name]

    if nil == taskGroup then
        error(string.format("[%s:%s] unknown task name :%s",self.package.Name,self.package.Version,name))
    end

    for _, taskgroup in ipairs(self:topSort(taskGroup)) do

        logger:I("invoke task(%s)",taskgroup.Name)

        for _,task in ipairs(taskgroup) do
            logger:D("\tfrom package [%s:%s] ...",task.Package.Name,task.Package.Version)
            task.F(...)
            logger:D("\tfrom package [%s:%s] -- success",task.Package.Name,task.Package.Version)
        end

        logger:I("invoke task(%s) -- success",taskgroup.Name)
    end

end

function module:topSort(taskGroup)

    if taskGroup.mark == "black" then return end

    if taskGroup.mark == "gray" then

        local errmsg = "DCG detected:"

        local flag = false

        for _,curr in ipairs(self.checkerOfDCG) do

            if curr == taskGroup then flag = true end

            if flag then
                errmsg = string.format("%s\n\t%s ->",errmsg, curr.Name)
            end

        end

        errmsg = string.format("%s\n\t%s",errmsg, taskGroup.Name)

        error(errmsg)
    end

    local sortGroups = {}

    taskGroup.mark = "gray"

    table.insert(self.checkerOfDCG,taskGroup)

    for _,task in ipairs(taskGroup) do
        logger:D("topshort task(%s) from package [%s:%s] ;%s",task.Name,task.Package.Name,task.Package.Version,task.Desc)

        if task.Prev ~= nil and task.Prev ~= "" then
            local prev = self.taskGroups[task.Prev]

            if prev == nil then
                error(string.format("unknown previous task(%s) for task(%s) from package [%s:%s]",task.Prev,task.Name,task.Package.Name,task.Package.Version))
            end

            local childSortGroups = self:topSort(prev)

            if childSortGroups ~= nil then
                table.insert(sortGroups,childSortGroups)
            end
        end
    end

    table.insert(sortGroups,taskGroup)

    table.remove(self.checkerOfDCG,#self.checkerOfDCG)

    return sortGroups
end


return module

