local lake = require "lake.lake"
local fs = require "lake.fs"
local log = require "lake.log"
local package = require "lake.package"

log.I("start a new lake process ...")

lake.WORKSPACE = fs.current()

local lakefile = lake.WORKSPACE .. "/.gsmake.lua"

if not fs.exists(lakefile) then
    lake.WORKSPACE = os.getenv("LAKE_HOME")
end

package.load(lake.WORKSPACE)

log.I("lake process complete")
