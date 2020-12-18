class route {
    constructor() {
        this.route_list = new Array();
    }
    add(method, pattern, callback) {
        this.route_list.push({
            'method': method,
            'pattern': pattern,
            'callback': callback,
            'regex': new RegExp(pattern)
        });
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
        for (var index in this.route_list) {
            let ele = this.route_list[index];
            if (ele.method.indexOf(m.method()) >= 0) {
                let param = ele.regex.exec(m.uri());
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