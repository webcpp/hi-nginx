<?php

require_once 'hi/servlet.php';


class  form implements \hi\servlet{
    public function handler(\hi\request &$req,\hi\response &$res){
        $res->content = "";
        foreach ($req->form as $key => $value) {
            $res->content .= $key ."=".$value."<br />";
        }
        $res->status =200;
    }
}
