import json
from hi import dbhelp
from website.index import db_setting

def find(req,res,param):
    db = dbhelp(**db_setting)
    try:
        if req.has_form("id"):
            count = db.execute("SELECT * FROM `websites` WHERE `id`=%s;" , (int(req.get_form('id'))))
            if count>0:
                result = db.fetchone()
                res.header('Content-Type','application/json')
                res.content(json.dumps(result,ensure_ascii=False))
                res.status(200)
        db.close()
    except Exception as e:
        db.close()
        res.content(repr(e))
        res.status(500)

def handler(req,res,param):
    find(req,res,param)

