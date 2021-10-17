<?php

	include_once 'ConnectToDataBase.php';

	$Data = json_decode(file_get_contents('php://input'));
	$id = $Data->id;

	$stmt = $Connect->prepare('Delete From servers where ServerID = ?');
	$stmt->bind_param("s", $id);
	$stmt->execute();

	if($stmt->fetch())
	{
		echo json_encode(array('Result' => true));
	}
	else
	{
		echo json_encode(array('Result' => false, 'Error' => 'The server could not be deleted. There is no server with this id'));
	}