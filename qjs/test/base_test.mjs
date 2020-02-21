import * as hi from "hi";

var base_test = function (m) {
    m.status(200);
    m.header('Content-Type', 'text/plain;charset=utf-8');
    m.content('hello,world\n');
};

export default base_test;