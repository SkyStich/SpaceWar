<?php 

include_once 'ConnectToDataBase.php';

$Data = json_decode(file_get_contents('php://input'));
$MapList = array();

foreach($Data->MapNames as $NameMap)
{
	$MapList[] = $NameMap->Map;
}

$ServerNames = array();
$ServerAddresses = array();
$MapNamesArray = array();

$In = str_repeat('?, ', count($MapList) - 1) . '?';

$stmt = $Connect->prepare("Select ServerName, Address, MapName From servers Where MapName = ($In)");
if($stmt)
{
	$types = str_repeat('s', count($MapList));
	$stmt->bind_param($types, ...$MapList);
	$stmt->execute();
	$stmt->bind_result($Name, $Address, $MapName);

	while($stmt->fetch())
	{
		$ServerNames[] = $Name;
		$ServerAddresses[] = $Address;
		$MapNamesArray[] = $MapName;
	}	
}

echo json_encode(array('ServerNames' => $ServerNames, 'ServerAddresses' => $ServerAddresses, 'MapNames' => $MapNamesArray));

?>