from hi import hi
import os
import sys
import importlib

sys.path.insert(0, os.path.dirname(os.path.realpath(__file__)))

app = hi()


@app.route(r'^/(?P<module>.+)/?$',['GET','POST','PUT','DELETE','PATCH'])
def controller(req,res,param):
    try:
        module = importlib.import_module('.index',param['module'].replace("/","."))
        module.handler(req,res,param)
    except Exception as e:
        res.content(repr(e))
        res.status(500)



def main(req, res):
    app.run(req, res)
