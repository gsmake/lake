name "github.com/gsmake/lua" -- package name

local class     = require "lemoon.class"
local logger    = class.new("lemoon.log","lake")


task.install = function()
    logger:I("call install ...")
end


task.install.Desc = "clang bootstrap install task"




