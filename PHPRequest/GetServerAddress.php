<?php
    $ip = file_get_contents('https://api.ipify.org');
    echo json_encode(array('ip' => $ip));
?>