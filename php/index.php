<?php

require_once 'hi/servlet.php';
require_once 'hi/route.php';

class index implements \hi\servlet {

    public function handler(\hi\request $req, \hi\response $res) {
        $app = \hi\route::get_instance();
        $app->add('{^/$}', array('GET'), function ($rq, $rs, &$param) {
            $rs->content = 'hello,world';
            $rs->status = 200;
        });

        $app->add('{^/who/(?P<name>\w+)/?$}', array('GET'), function ($rq, $rs, &$param) {
            $rs->content = 'hello,' . $param['name'];
            $rs->status = 200;
        });

        $app->add('{^/phpinfo/?$}', array('GET'), function($rq, $rs, &$param) {
            ob_start();
            phpinfo();
            $rs->content = ob_get_contents();
            $rs->status = 200;
            ob_end_clean();
        });

        $app->add('{^/cache/?$}', array('GET'), function($rq, $rs, &$param) {
            $key = 'cache_test';
            $value = 0;
            if (array_key_exists($key, $rq->cache)) {
                $value = intval($rq->cache[$key]) + 1;
            }
            $rs->cache[$key] = $value;
            $rs->content = "$key : " . $value;
            $rs->status = 200;
        });

        $app->add('{^/(?P<file>\w+)/?$}', array('GET'), function($rq, $rs, &$param) {
            $this->data['title'] = 'title';
            $this->data['body'] = 'body';
            $tpl_file = dirname(__FILE__) . '/templates/' . $param['file'] . '.php';
            if (file_exists($tpl_file)) {
                $rs->headers['Content-Type'] = 'text/html;charset=utf-8';
                $rs->status = 200;
                $rs->content = $this->render($tpl_file);
            } else {
                $rs->headers['Content-Type'] = 'text/plain;charset=utf-8';
                $rs->status = 400;
                $rs->content = 'Not Found:' . $tpl_file;
            }
        });

        $app->run($req, $res);
    }

    private function render($file) {
        $result = '';
        ob_start();
        include $file;
        $result = ob_get_contents();
        ob_end_clean();
        return $result;
    }

    private $data = array();

}
