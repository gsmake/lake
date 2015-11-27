local fs = require "lemoon.fs"

print(fs.abs("./"))

print(fs.dir())

local dir = fs.dir() .. "/lua_dir"

if fs.exists(dir) then
    fs.rm(dir,true)
end

fs.mkdir(dir)

fs.mkdir(dir .. "/test")

local dir_link = fs.dir() .. "/lua_dir_link"

fs.list(fs.dir(),function(entry)
    print("child entry :" .. entry)
end)

if fs.exists(dir_link) then
    fs.rm(dir_link)
end

fs.symlink(dir,dir_link)

fs.dir("/usr/local/bin")

print(fs.dir())

print(fs.isdir("/usr/local2"))

print(fs.exists("/usr/local2"))


print(fs.abs("./"))

