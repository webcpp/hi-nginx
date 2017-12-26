<?php

//error_reporting(0);
error_reporting(E_ERROR | E_WARNING | E_PARSE);


require_once 'hi/servlet.php';

class demo implements \hi\servlet {

    private $data = array();

    public function __construct() {
        
    }

    public function __destruct() {
        
    }

    public function handler(\hi\request &$req, \hi\response &$res) {
        $this->data['name'] = 'pangpang';
        $this->data['title'] = 'php template engine';
        $res->content = $this->render('templates/demo.tpl.php');
        $res->status = 200;
    }

    private function render($tpl_file) {
        $result = '';
        ob_start();
        include dirname(__FILE__) . '/' . $tpl_file;
        $result = ob_get_contents();
        ob_end_clean();
        return $result;
    }

}
