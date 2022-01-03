local m = {}

function m.handler(req, res)
    res:set_header('Content-Type', 'text/plain;charset=UTF-8')
    res.status = 200
    res.content = "welcome to lua\n"
end

return m
