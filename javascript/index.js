if (typeof (Mustache) == 'undefined') {
    load('https://cdn.bootcss.com/mustache.js/2.3.0/mustache.min.js')
}


var list = java.util.Arrays.asList(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
var template = "{{#list}}* {{.}}\n{{/list}}"
var key = 'test', output
if (hi_req.cache.containsKey(key)) {
    output = hi_req.cache.get(key)
} else {
    output = Mustache.render(template, {'list': JSON.parse(list.toString())})
    hi_res.cache.put(key, output)
}
hi_res.headers.get('Content-Type').set(0, 'text/plain;charset=UTF-8')
hi_res.content = output
hi_res.status = 200;
