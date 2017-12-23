<?php

error_reporting(E_ERROR | E_WARNING | E_PARSE);
require_once dirname(dirname(__FILE__)).'/hi/servlet.php';

class detail implements \hi\servlet {

    public function handler(\hi\request &$req, \hi\response &$res) {
        $res->content = 'blog detail';
        $res->status = 200;
    }

}
