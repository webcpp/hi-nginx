--[[
hi_res:content('hello,world')
hi_res:status(200)
hi_res:header('Content-Type','text/plain;charset=utf-8')
--]]

local route=require('route'):get_instance()


route:add({'GET'},'^/test/?$'
,function(req,res,param)
    res:content('hello,world')
    res:header('Content-Type','text/plain;charset=UTF-8')
    res:status(200)
end)

route:add({'GET'},'^/hello/(%w+)/?$'
,function(req,res,param)
    res:content('hello,'..param[1])
    res:header('Content-Type','text/plain;charset=UTF-8')
    res:status(200)
end)


route:run(hi_req,hi_res)
