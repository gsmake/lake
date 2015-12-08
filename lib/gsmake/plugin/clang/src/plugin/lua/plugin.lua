local class = require "lemoon.class"

local logger = class.new("lemoon.log","clang")


task.gencmake = function(self)

    logger:I("%s",package.path)

    local cmakegen = class.new("cmakegen")

    logger:D("generate cmake project files")

end



task.compile = function(self)

end
task.compile.Prev = "gencmake"


task.install = function(self,target)

end
task.install.Prev = "compile"

