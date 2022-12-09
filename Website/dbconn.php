<?PHP
/*
 * PERSON1: Ehren Grenlund
 * PERSON2:
 * PERSON3:
 * CSCI466 - 003
 * Group Assignment 
 */
//Credential variables to login to the SQL server
$host = 'students';								                        //Hostname
$user = 'z1861935';								                        //Username
$password = '2000Nov06';						                        //Password

$db = 'z1861935';								                        //Database name we are connecting to
$newdb = new PDO("mysql:host=$host;dbname=$db", $user, $password);
$legacydb = new PDO("mysql:host=blitz.cs.niu.edu;dbname=csci467;port=3306", "student", "student");

try {
	$newdb->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);		//Try to establish a connection to the server
	$legacydb->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
}
catch (PDOException $e) {
	echo 'ERROR: ' . $e->getMessage();					                //If it fails, let the user know
}
?>
