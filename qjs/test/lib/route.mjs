var route = function () {
    this.instance = null;
    this.map = new Map();
    this.add = function (method, pattern, callback) {
        if (!this.map.has(pattern)) {
            var ele = {};
            ele.method = typeof method == 'object' ? method : [method];
            ele.pattern = pattern;
            ele.callback = callback;
            this.map.set(pattern, ele);
        }
    }

    this.get = function (pattern, callback) {
        this.add(['GET'], pattern, callback);
    };

    this.post = function (pattern, callback) {
        this.add(['POST'], pattern, callback);
    };

    this.put = function (pattern, callback) {
        this.add(['PUT'], pattern, callback);
    };

    this.head = function (pattern, callback) {
        this.add(['HEAD'], pattern, callback);
    };

    this.delete = function (pattern, callback) {
        this.add(['DELETE'], pattern, callback);
    }

    this.patch = function (pattern, callback) {
        this.add(['PATCH'], pattern, callback);
    }

    this.options = function(pattern,callback){
        this.add(['OPTIONS'],pattern,callback);
    };

    this.all = function (pattern, callback) {
        this.add(['GET', 'POST', 'PUT', 'HEAD', 'DELETE', 'PATCH','OPTIONS'], pattern, callback);
    }

    this.run = function (m) {
        for (let [pattern, ele] of this.map.entries()) {
            if (ele.method.indexOf(m.method()) >= 0) {
                let reg = new RegExp(pattern, 'ig');
                let param = reg.exec(m.uri());
                if (param != null) {
                    ele.callback(m, param);
                    break;
                }
            }
        }
    }
}

route.get_instance = function () {
    if (this.instance == null) {
        this.instance = new route();
    }
    return this.instance;
}

export default route;