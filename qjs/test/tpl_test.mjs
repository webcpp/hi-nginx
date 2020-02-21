import * as hi from "hi";
import * as std from 'std';
import * as os from 'os';
import Mustache from "./lib/mustache.mjs"

var view = {
    "users": ['a', 'b', 'c', 'd']
};


var tpl_map = new Map();

var root_path = './qjs/test/tpl/';

var tpl_test = function (m) {
    m.status(200);
    m.header('Content-Type', 'text/html;charset=utf-8');
    if (!tpl_map.has('base')) {
        let path = root_path + 'base.tpl.html';
        let f = std.open(path, 'r');
        tpl_map.set('base', f.readAsString());
        f.close();
    }
    m.content(Mustache.render(tpl_map.get('base'), view, function (name) {
        if (!tpl_map.has(name)) {
            let f = std.open(root_path + name + '.tpl.html', 'r');
            tpl_map.set(name, f.readAsString());
            f.close();
        }
        return tpl_map.get(name);
    }));
};


export default tpl_test;

