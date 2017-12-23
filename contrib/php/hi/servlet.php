<?php

require_once 'request.php';
require_once 'response.php';

interface servlet {

    public function handler(\hi\request &$req, \hi\response &$res);
}
