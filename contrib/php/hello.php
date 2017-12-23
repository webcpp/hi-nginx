<?php

require_once 'hi/servlet.php';

class hello implements \hi\servlet {

    public function handler(\hi\request &$req, \hi\response &$res) {
        $res->content = 'hello,world';
        $res->status = 200;
    }

}
