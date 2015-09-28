-- load package
local log = require "lake.log"
local fs = require "lake.fs"
local strings = require "lake.strings"
local vfs = require "lake.vfs"

local module = {}

local loading = require "lake.list"

function module.load(path)
    log.D("loading package :" .. path)

    local lakefile = path .. "/.gsmake.lua"

    if not fs.exists(lakefile) then
        log.I("skip load package : .gsmake.lua not found\n\tpath :" .. path)
    end

    local metadata = dofile(lakefile)

    if metadata == nil then
        error(string.format("loading %s error : expect return metadata",lakefile))
    end

    log.D("--------------------------------------------------")

    log.D("loading package ...")

    log.D("name    :%s", metadata.name)

    log.D("domain  :%s", strings.printenum(metadata.domain))

    for _,import in ipairs(metadata.imports) do

        if import.name == nil or import.name == "" then
            error("package name must set")
        end

        if import.domain == nil then
            import.domain = metadata.domain
        end

        if import.version == nil then
            import.version = "master"
        end

        log.D("import  :{ name :%s, domain :%s, version :%s }",import.name, strings.printenum(import.domain),import.version)
    end

    for _,task in ipairs(metadata.tasks) do

        if task.domain == nil then
            task.domain = metadata.domain
        end

        log.D("task    : %s -> %s ;%s", task.name, strings.printenum(task.domain), task.description)
    end

    log.D("--------------------------------------------------")


    vfs.open(metadata.name)

    -- load imports



    --log.I(tostring(metadata))
end

return module
