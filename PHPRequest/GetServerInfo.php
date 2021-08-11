<?php

include_once 'ConnectToDataBase.php';

$Data = json_decode(file_get_contents('php://input'));
$ServerId = $Data->id;

$stmt = $Connect->prepare("Select ServerName, Address From Servers Where ServerId = ?");
$stmt->bind_param("s", $ServerId);
$stmt->execute();
$stmt->bind_result($ServerName, $Address);
if($stmt->fetch())
{
	echo json_encode(array('ServerName' => $ServerName, 'Address' => $Address));
}
else
{
	echo json_encode(array("Result" => false, "Error" => 'Server not fount in data base'));
}