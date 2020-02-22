class route {
    constructor() {
        this.map = new Map();
    }
    add(method, pattern, callback) {
        if (!this.map.has(pattern)) {
            var ele = {};
            ele.method = typeof method == 'object' ? method : [method];
            ele.pattern = pattern;
            ele.callback = callback;
            this.map.set(pattern, ele);
        }
    }

    get(pattern, callback) {
        this.add(['GET'], pattern, callback);
    }

    post(pattern, callback) {
        this.add(['POST'], pattern, callback);
    }

    put(pattern, callback) {
        this.add(['PUT'], pattern, callback);
    }

    head(pattern, callback) {
        this.add(['HEAD'], pattern, callback);
    }

    delete(pattern, callback) {
        this.add(['DELETE'], pattern, callback);
    };

    patch(pattern, callback) {
        this.add(['PATCH'], pattern, callback);
    }

    options(pattern, callback) {
        this.add(['OPTIONS'], pattern, callback);
    }

    all(pattern, callback) {
        this.add(['GET', 'POST', 'PUT', 'HEAD', 'DELETE', 'PATCH', 'OPTIONS'], pattern, callback);
    }

    run(m) {
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
    static get_instance() {
        if (typeof (route.instance) == 'undefined') {
            route.instance = new route();
        }
        return route.instance;
    }
}

export default route;