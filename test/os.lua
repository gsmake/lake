--
-- Created by IntelliJ IDEA.
-- User: liyang
-- Date: 15/11/26
-- Time: 下午5:27
-- To change this template use File | Settings | File Templates.
--

local sys = require "lemoon.os"

local exec = sys.exec("ls")

exec:start()

print("========")
exec:wait()
print("========")

sys = nil


collectgarbage()