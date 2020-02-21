import * as hi from "hi";
import * as hash from "hash";
import route from "./lib/route.mjs"


var r = route.get_instance();

r.get('^/(.*)/?$', function (m, param) {
    m.header('Content-Type', 'text/plain;charset=UTF8')
    let content = m.uri() + "\n" + hash.md5(m.uri()) + '\n' + hash.sha1(m.uri()) + '\n' + hash.sha256(m.uri())+"\n" + hash.sha512(m.uri()) + "\n";
    m.content(content);
    m.status(200)
});

var route_test = function (m) {
    r.run(m)
};


export default route_test;