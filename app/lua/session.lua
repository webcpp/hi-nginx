local m = {}

function m.handler(req, res)
    res:set_header('Content-Type', 'text/plain;charset=UTF-8')
    local c = 0
    if req:has_cookie('test') then
        c = tonumber(req:get_cookie('test'))+1
        res.content = tostring(c)
    else
        res.content = 'not found\n'
    end
    res:set_cookie('test',tostring(c),'Max-Age=300; Path=/; Domain=localhost')
    res.status = 200
end

return m