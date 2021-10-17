<?php

include_once 'ConnectToDataBase.php';

$Data = json_decode(file_get_contents('php://input'));

$ServerName = $Data->Servername;
$Address = $Data->Address;
$MapName = $Data->MapName;

$stmt = $Connect->prepare("Select ServerId From Servers where Address = ?");
$stmt->bind_param("s", $Address);
$stmt->execute();

if($stmt->fetch())
{
	echo json_encode(array('ServerId' => -1, "Error" => 'Server already exists'));
}
else
{
	$stmt->close();
	$stmt = $Connect->prepare("Insert into Servers(ServerName, Address, MapName) Values (?, ?, ?)");
	$stmt->bind_param("sss", $ServerName, $Address, $MapName);
	$stmt->execute();

	$LastId = $stmt->insert_id;
	echo json_encode(Array('ServerId' => $LastId));
}