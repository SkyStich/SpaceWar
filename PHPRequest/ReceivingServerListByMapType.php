<?php 

include_once 'ConnectToDataBase.php';

$Data = json_decode(file_get_contents('php://input'));
$MapType = $Data->GameType;

$stmt = $Connect->prepare("Select ServerName, Address, MapName From servers Where MapType = ?");
$stmt->bind_param('s', $MapType);
$stmt->execute();
$stmt->bind_result($Name, $Address, $MapName);

$ServerNames = array();
$ServerAddresses = array();
$MapNamesArray = array();

while($stmt->fetch())
{
	$ServerNames[] = $Name;
	$ServerAddresses[] = $Address;
	$MapNamesArray[] = $MapName;
}
echo json_encode(array('ServerNames' => $ServerNames, 'ServerAddresses' => $ServerAddresses, 'MapNames' => $MapNamesArray));