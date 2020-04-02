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
        tpl_map.set('base', std.loadFile(root_path + 'base.tpl.html'));
    }
    m.content(Mustache.render(tpl_map.get('base'), view, function (name) {
        if (!tpl_map.has(name)) {
            tpl_map.set(name, std.loadFile(root_path + name + '.tpl.html'));
        }
        return tpl_map.get(name);
    }));
};


export default tpl_test;

