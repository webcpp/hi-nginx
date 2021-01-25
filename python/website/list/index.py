import json
from hi import dbhelp
from website.index import db_setting


def findall(req,res,param):
    db = dbhelp(**db_setting)
    try:
        order = 'DESC'
        start = 0
        size = 5
        if req.has_form('order'):
            order = req.get_form('order')
        if(req.has_form('start')):
            start = int(req.get_form('start'))
        if(req.has_form('size')):
            size = int(req.get_form('size'))
        count = db.execute("SELECT * FROM `websites` ORDER BY `id` {} LIMIT %s,%s;".format(order),(start,size))
        if count >0:
            result = db.fetchall()
            res.header('Content-Type','application/json')
            res.content(json.dumps(result,ensure_ascii=False))
            res.status(200)
        db.close()
    except Exception as e:
        db.close()
        res.content(repr(e))
        res.status(500)

def handler(req,res,param):
    findall(req,res,param)

