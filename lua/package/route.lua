local route = {map = {}, instance = nil}

function route:new()
    local cls = {}
    setmetatable(cls, self)
    self.__index = self
    return cls
end

function route:get_instance()
    if self.instance == nil then
        self.instance = self:new()
    end
    return self.instance
end

function route:free()
    if self.instance then
        self.map = nil
        self.map = {}
        self.instance = nil
    end
end

function route:add(method, pattern, callback)
    local ele = {}
    ele.method = method
    ele.pattern = pattern
    ele.callback = callback
    self.map[#(self.map) + 1] = ele
end

function route:run(req, res)
    local uri = req:uri()
    for i = 1, #(self.map) do
        local ele = self.map[i]
        for j, m in pairs(ele.method) do
            if m == req:method() then
                local param = string.match(uri, ele.pattern)
                if param then
                    ele.callback(req, res, {param})
                    return
                end
            end
        end
    end
end

return route
