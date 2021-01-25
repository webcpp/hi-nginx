def handler(req,res,param):
    res.header('Content-Type','text/plain;charset=utf-8')
    res.content('welcome to hi.py\n')
    res.status(200)