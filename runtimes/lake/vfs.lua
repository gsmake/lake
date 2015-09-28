local log  = require "lake.log"
local lake = require "lake.lake"
local fs = require "lake.fs"


local metadatadir = lake.HOME_PATH .. "/metadata/"

if not fs.exists(metadatadir) then
    fs.mkdir(metadatadir)
end

local capi = __vfs.open(lake.HOME_PATH .. "/metadata/lake.db")


local module = {}
function module.open(packagename)
    log.D("open package(%s) vfs space ...",packagename)
end


return module
