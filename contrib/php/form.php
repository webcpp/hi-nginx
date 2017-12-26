<?php

require_once 'hi/servlet.php';

class form implements \hi\servlet {

    public function __construct() {
        
    }

    public function __destruct() {
        
    }

    public function handler(\hi\request &$req, \hi\response &$res) {
        $res->content = "form :<br />";
        foreach ($req->form as $key => $value) {
            $res->content .= $key . "=" . $value . "<br />";
        }
        $res->content .= "headers :<br />";
        foreach ($req->headers as $key => $value) {
            $res->content .= $key . "=" . $value . "<br />";
        }
        $res->content .= "cookies :<br />";
        foreach ($req->cookies as $key => $value) {
            $res->content .= $key . "=" . $value . "<br />";
        }
        $res->content .= "session :<br />";
        foreach ($req->session as $key => $value) {
            $res->content .= $key . "=" . $value . "<br />";
        }
        $res->status = 200;
    }

}
