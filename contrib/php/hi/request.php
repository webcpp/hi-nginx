<?php

namespace hi;

class request {

    public $client = "", $method = "", $uri = "", $user_agent = "", $param = "";
    public $headers = array(), $form = array(), $cookies = array(), $session = array();

    public function __construct() {
        
    }

    public function __destruct() {
        
    }

}
