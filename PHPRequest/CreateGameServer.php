<?php

include_once 'ConnectToDataBase.php';

$Data = json_decode(file_get_contents('php://input'));

$ServerName = $Data->Servername;
$Address = $Data->Address;

$stmt = $Connect->prepare("Select ServerId From Servers where ServerName = ?");
$stmt->bind_param("s", $ServerName);
$stmt->execute();
$stmt->bind_result($id);

if($stmt->fetch())
{
	echo json_encode(array('ServerId' => -1, "Error" => 'Server already exists'));
}
else
{
	$stmt->close();
	$stmt = $Connect->prepare("Insert into Servers(ServerName, Address) Values (?, ?)");
	$stmt->bind_param("ss", $ServerName, $Address);
	$stmt->execute();

	json_encode(Array('ServerId' => $id));
}