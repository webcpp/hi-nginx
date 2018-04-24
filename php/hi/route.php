<?php

namespace hi;

class route {

    private function __construct() {
        
    }

    public function add($pattern, $method, $callback) {
        if (!array_key_exists($pattern, $this->uri_map)) {
            $this->uri_map[$pattern] = array('method' => $method, 'callback' => $callback);
        }
    }

    public function run($req, $res) {
        foreach ($this->uri_map as $k => $v) {
            if (in_array($req->method, $v['method'])) {
                $matches = array();
                if (preg_match($k, $req->uri, $matches)) {
                    $v['callback']($req, $res, $matches);
                    break;
                }
            }
        }
    }

    private $uri_map = array();
    private static $instance = null;

    public static function get_instance() {
        if (is_null(\hi\route::$instance)) {
            \hi\route::$instance = new \hi\route();
        }
        return \hi\route::$instance;
    }

}
