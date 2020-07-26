local example = {route = nil, instance = nil}

function example:new()
    self.route = require("route"):get_instance()
    self.route:add(
        {"GET"},
        "^/test/?$",
        function(req, res, param)
            res:content("hello,world")
            res:header("Content-Type", "text/plain;charset=UTF-8")
            res:status(200)
        end
    )

    self.route:add(
        {"GET"},
        "^/hello/(%w+)/?$",
        function(req, res, param)
            res:content("hello," .. param[1])
            res:header("Content-Type", "text/plain;charset=UTF-8")
            res:status(200)
        end
    )

    local cls = {}
    setmetatable(cls, self)
    self.__index = self
    return cls
end

function example:get_instance()
    if self.instance == nil then
        self.instance = self:new()
    end
    return self.instance
end

function example:main(req, res)
    self.route:run(req, res)
end

return example
