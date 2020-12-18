import * as hi from "hi";
import {
  crypto
} from "crypto";
import route from "./lib/route.mjs"

var r = route.get_instance();

r.get('^/test/?$', function (m, param) {
  m.status(200);
  m.header('Content-Type', 'text/plain;charset=utf-8');
  m.content('hello,world\n');
});

r.get('^/hello/(.*)/?$', function (m, param) {
  m.header('Content-Type', 'text/plain;charset=UTF8')
  m.content(m.method() + '\n' + m.uri() + '\n' + param.toString())
  m.status(200)
});

var route_test = function (m) {
  r.run(m)
};


export default route_test;