<?php

include_once 'ConnectToDataBase.php';

$Data = json_decode(file_get_contents('php://input'));

$ServerName = $Data->ServerName;

$stmt = $Connect->prepare("Select Address From Servers Where ServerName = ?");
$stmt->bind_param("s", $ServerName);
$stmt->execute();
$stmt->bind_result($Address);

if($stmt->fetch())
{
	echo json_encode(array('Result' => true, 'ip' => $Address));
}
else
{
	echo json_encode(array('Result' => false));
}