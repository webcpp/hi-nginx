var route = function () {
    this.instance = null
    this.map = {}
    this.add = function (method, pattern, callback) {
        if (typeof (this.map[pattern]) == "undefined") {
            var ele = {}
            ele.method = method
            ele.pattern = pattern
            ele.callback = callback
            this.map[pattern] = ele
        }
    }

    this.get = function (pattern, callback) {
        this.add(['GET'], pattern, callback)
    }
    
    this.post = function (pattern, callback) {
        this.add(['POST'], pattern, callback)
    }

    this.put = function (pattern, callback) {
        this.add(['PUT'], pattern, callback)
    }
    
    this.head = function (pattern, callback) {
        this.add(['HEAD'], pattern, callback)
    }

    this.run = function (req, res, param) {
        var pattern = null
        for (pattern in this.map) {
            var reg = new RegExp(pattern, 'ig');
            var param = reg.exec(req.uri())
            if (param != null) {
                var ele = this.map[pattern]
                if (ele.method.indexOf(req.method()) >= 0) {
                    ele.callback(req, res, param)
                    break
                }
            }
        }
    }
}

route.get_instance = function () {
    if (this.instance == null) {
        this.instance = new route()
    }
    return this.instance;
}

module.exports = route
