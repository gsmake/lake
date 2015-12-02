local fs = require "lemoon.fs"
local sys = require "lemoon.sys"

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

if sys.host() == "Win32" or sys.host() == "Win64" then
    fs.dir("c:/")
else
    fs.dir("/usr/local/bin")
end

print(fs.dir())

print(fs.isdir("/usr/local2"))

print(fs.exists("/usr/local2"))


print(fs.abs("./"))
