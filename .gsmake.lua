name "github.com/gsmake/lake" -- package name

version "master" -- package version

plugin "github.com/gsmake/clang"

--plugin "github.com/gsmake/lua"

task.install = function()

end

task.install.prev = "compile"

task.install.Description = "gsmake bootstrap install command"

