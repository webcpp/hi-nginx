<?php

error_reporting(E_ERROR | E_WARNING | E_PARSE);
require_once 'hi/servlet.php';

class phpinfo implements \hi\servlet {

    public function __construct() {
        
    }

    public function __destruct() {
        
    }

    public function handler(\hi\request &$req, \hi\response &$res) {
        ob_start();
        echo phpinfo();
        $res->content = ob_get_contents();
        $res->status = 200;
        ob_end_clean();
    }

}
