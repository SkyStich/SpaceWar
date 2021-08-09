<?php

include_once 'ConnectToDataBase.php';

$MyData = json_decode(file_get_contents('php://input'));
$Login = $MyData->Login;
$Pass = $MyData->Password;

$stmt = $Connect->prepare("Select UserId From UserData Where PlayerLogin = ? and Pass = ?");
$stmt->bind_param("ss", $Login, $Pass);
$stmt->execute();

if($stmt->fetch())
{
	$stmt->close();

	echo json_encode(array('AuthorizationResult' => true));
}
else
{
	echo json_encode(array('AuthorizationResult' => false, 'Error' => 'invalid username or password ' . $Login . '    ' . $Pass));
}
