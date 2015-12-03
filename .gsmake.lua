name "github.com/gsmake/lake" -- package name

version "master" -- package version

plugin "github.com/gsmake/clang" : version "bootstrap"

plugin "github.com/gsmake/lua" : version "bootstrap"


task.install = function()

end

task.install.prev = "compile"

task.install.Description = "gsmake bootstrap install command"

