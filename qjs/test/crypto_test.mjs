import * as hi from "hi";
import { crypto } from "crypto";
import route from "./lib/route.mjs"

var r = route.get_instance();

r.get('^/(.*)/?$', function (m, param) {
    m.header('Content-Type', 'text/plain;charset=UTF8')
    let aes = new crypto('123456');
    let cipher = aes.encode(m.uri());
    let plain = aes.decode(cipher);
    m.content(cipher + '\n' + plain + '\n');
    m.status(200)
});

var route_test = function (m) {
    r.run(m)
};


export default route_test;