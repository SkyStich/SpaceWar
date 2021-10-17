<?php 

include_once 'ConnectToDataBase.php';

$Data = json_decode(file_get_contents('php://input'));
$Login = $Data->Login;

$stmt = $Connect->prepare('Select WeaponName From PlayerWeapon Where UserId = (Select UserId From UserData Where PlayerLogin = ?)');
$stmt->bind_param("s", $Login);
$stmt->execute();
$stmt->bind_result($Name);

$List = array(); 

while($stmt->fetch())
{
	$List[] = $Name;
}

echo json_encode(array('WeaponList' => $List));