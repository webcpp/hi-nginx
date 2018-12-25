/*
 var foo = require('foo')
 hi_res.header('Content-Type','text/plain;charset=UTF-8')
 hi_res.content(foo.hello())
 hi_res.status(200)
*/


/*
 var mustache = require('mustache')
 var template = '<h1>Test mustache engine</h1><p>{{body}}</p>';
 var model = {body:'Hello World'};
 hi_res.content(mustache.to_html(template,model))
 hi_res.status(200)
 */

/*
 var handlebars = require('handlebars')
 var template = handlebars.compile('<h1>Test handlebars engine</h1><p>{{body}}</p>')
 var model = {body:'Hello World'};
 hi_res.content(template(model))
 hi_res.status(200)
 */


/*
 var echo = require('echo')
 var t=new echo()
 t.set('HELLO, ')
 hi_res.content(t.concat('Tom'))
 hi_res.status(200)
 */



/*

var route = require('route').get_instance()

route.get('^\/get/?(\\w+)$', function (req, res, param) {
    res.header('Content-Type', 'text/plain;charset=UTF8')
    res.content(req.method()+'\n'+req.uri() + '\n' + param.toString())
    res.status(200)
})

route.post('^\/post/?([a-zA-Z]+)?$', function (req, res, param) {
    res.header('Content-Type', 'text/plain;charset=UTF8')
    res.content(req.method()+'\n'+req.uri() + '\n' + param.toString())
    res.status(200)
})

route.put('^\/put/?([0-9]+)?', function (req, res, param) {
    res.header('Content-Type', 'text/plain;charset=UTF8')
    res.content(req.method()+'\n'+req.uri() + '\n' + param.toString())
    res.status(200)
})

route.add(['GET', 'POST', 'PUT'], '^\/(.*)$', function (req, res, param) {
    res.header('Content-Type', 'text/plain;charset=UTF8')
    res.content(req.method()+'\n'+req.uri() + '\n' + param.toString())
    res.status(200)
})

route.run(hi_req, hi_res)



*/


///*
 hi_res.header('Content-Type','text/plain;charset=UTF-8')
 hi_res.content('hello,world')
 hi_res.status(200)
//*/
