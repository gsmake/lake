fs = lemoon.fs() -- create new fs module

lake = {
    home = os.getenv("LAKE_HOME"),
    workdir = fs.current()
}

print("[inf] lake work directory :",lake.workdir)

local lakefile = lake.workdir .. "/.gsmake.lua"

if fs.exists(lakefile) == false then
    print("[err] lake file not exists : "..lakefile)
    return
end

dofile(lakefile)

if not fs.exists([==[d:\test]==]) then
    fs.create_directory_symlink(lake.workdir,[==[d:\test]==])
end
