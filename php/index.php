<?php



require_once 'hi/servlet.php';
require_once 'example/index.php';

class index implements \hi\servlet
{

    public function handler(\hi\request $req, \hi\response $res)
    {
        $app = \example\index::get_instance();

        $app->main($req, $res);
    }
}
