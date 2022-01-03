
def handler(req,res):
    res.set_header('Content-Type', 'text/plain;charset=UTF-8')
    res.content = 'welcome to python\n'
    res.status = 200