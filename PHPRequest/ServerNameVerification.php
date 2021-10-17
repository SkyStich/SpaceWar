<?php

include_once 'ConnectToDataBase.php';

$Data = json_decode(file_get_contents('php://input'));
$ServerName = $Data->ServerName;

$stmt = $Connect->prepare('select ServerName from servers where ServerName = ?');
$stmt->bind_param("s", $ServerName);
$stmt->execute();

echo json_encode(array('IsPassedVerification' => !$stmt->fetch()));