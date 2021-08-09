<?php

include_once 'ConnectToDataBase.php';

$MyData = json_decode(file_get_contents('php://input'));
$Login = $MyData->Login;
$Password = $MyData->Password;
$Email = $MyData->Email;

//Where '?' - parameneter
$stmt = $Connect->prepare("Select UserId From UserData Where PlayerLogin = ?");

//add parameter. Where "s" - string, $Login - parameter
$stmt->bind_param("s", $Login);

//perform the above
$stmt->execute();

//returns true if the user with this username already exists in the database
if($stmt->fetch())
{
	echo json_encode(array('RegisterResult' => false, 'Error' => 'a user with this username exists: ' . $Login));
}
else
{
	//close the previous request
	$stmt->close();

	//Create request for register
	$stmt = $Connect->prepare("Insert Into userdata (PlayerLogin, Pass, Email) Values(?, ?, ?)");
	$stmt->bind_param("sss", $Login, $Password, $Email);
	$stmt->execute();
	echo json_encode(array('RegisterResult' => true));
}

?>