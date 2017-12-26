<?php

namespace hi;

class response {

    public $status = 404, $content = "<p style='text-align:center;margin:100px;'>404 Not Found</p>";
    public $headers = array(), $session = array();

    public function __construct() {
        
    }

    public function __destruct() {
        
    }

}
