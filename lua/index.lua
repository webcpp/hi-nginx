--[[
hi_res:content("hello,world")
hi_res:status(200)
hi_res:header("Content-Type", "text/plain;charset=utf-8")
--]]
local index = require("example"):get_instance()
index:main(hi_req, hi_res)
