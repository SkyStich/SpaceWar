<?php

//Server Name (if the server is external, you need to find out its address from the hosting)
$ServerName = 'localhost';

//the name of the user who can connect to the database 
$UserName = 'Smurf';

//Pass from $UserName
$Password = 'dwGVwsxQmFTSgQRR';

//the name of the database to which the connection will take place
$DataBaseName = 'spacewar';

$Connect = new mysqli($ServerName, $UserName, $Password, $DataBaseName);

if($Connect->connect_error)
{
	echo(json_encode(array('Success'=> false, 'Error'=>'Connection failed: ' . $Connect->connect_error)));
	die;
}