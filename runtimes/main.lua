--
-- this file is gsmake boostrap lua script file
--

local log = require "lemoon.log" . get "lake"


log:E("hello world %s","a")

log:W("hello world %s","a")

log:I("hello world %s","a")

log:D("hello world %s","a")

log:T("hello world %s","a")

log:V("hello world %s","a")


log:exit()