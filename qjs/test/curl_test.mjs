import * as hi from "hi";
import * as std from 'std';

var curl_test = function (m) {
    let res = std.urlGet('https://www.baidu.com/', {
        full: true
    });
    m.status(res.status);
    m.header('Content-Type', 'text/plain;charset=utf-8');
    m.content(res.responseHeaders + res.response);
};

export default curl_test;
