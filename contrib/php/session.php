<?php

require_once 'hi/servlet.php';

class session implements \hi\servlet {

    public function __construct() {
        
    }

    public function __destruct() {
        
    }

    public function handler(\hi\request &$req, \hi\response &$res) {
        $key = 'test';
        $val = 0;
        if (array_key_exists($key, $req->session)) {
            $val = (int) $req->session[$key];
            $res->session[$key] = $val + 1;
        } else {
            $res->session[$key] = $val;
        }
        $res->content = 'hello,' . $val;
        $res->status = 200;
    }

}
