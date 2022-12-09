<html>

<head><title>Order Confirmation</title></head>

<p><a href="http://students.cs.niu.edu/~z1861935/467Project/">Go Back</a></p>
<?php

require("dbconn.php");
session_start();
$creditCardNumber = $_GET["creditCard"];
$name = $_GET["name"];
$email = $_GET["email"];
$address = $_GET["address"];
$expDate = $_GET["expDate"];
$kill = false;


$CCLength = strlen((string) $creditCardNumber);
if ($CCLength != 16){

	echo "INVALID CREDIT CARD NUMBER";
	$kill = true;
}else{
	$section4 = $creditCardNumber % 10000;
        $creditCardNumber = $creditCardNumber / 10000;
        $section3 = $creditCardNumber % 10000;
        $creditCardNumber = $creditCardNumber / 10000;
        $section2 = $creditCardNumber % 10000;
        $creditCardNumber = $creditCardNumber / 10000;
        $section1 = $creditCardNumber % 10000;
        $creditCardNumber = $creditCardNumber / 10000;

	$formatedCCNum = "$section1 $section2 $section3 $section4";
	#echo $formatedCCNum;

}

if($kill){ exit(1); }

#echo date('Y-m-d');

$sql = "INSERT INTO OrderPart (name, shipAddr, email, CCNum, CCExp, total, dateSub, weight) VALUES (?,?,?,?,?, ".$_GET['total'].", '".date("Y-m-d")."', ".$_GET['weight'].");";

$orderPartsStm = $newdb->prepare($sql);
$success = $orderPartsStm->execute(array($name, $address, $email, $_GET["creditCard"], $expDate."-1"));
	
$sql = "SELECT MAX(id) FROM OrderPart";
$orderPartsStm = $newdb->prepare($sql);
$success = $orderPartsStm->execute();

$orderId = $orderPartsStm->fetch( PDO::FETCH_ASSOC );

$sql = "INSERT INTO OrderContains VALUES (".$orderId["MAX(id)"].", ?, ?)";
$orderContainsStm = $newdb->prepare($sql);

$sql = "UPDATE Inventory SET qty = qty - ? WHERE partNum = ?;";
$updateInventoryStm = $newdb->prepare($sql);

foreach($_SESSION as $key => $value){
	$partNum = substr($key, 4);
	if($value != 0){
		$orderContainsStm->execute(array($partNum, $value));
		$updateInventoryStm->execute(array($value, $partNum));
	}
}

$ccExpSubmit = substr($expDate, -2)."/".substr($expDate, 0, 4);

#echo "<br>".$ccExpSubmit."<br>";

$sql = "SELECT MAX(id) FROM OrderPart;";
$maxIdStm = $newdb->prepare($sql);
$maxIdStm->execute();
$CCID = ($maxIdStm->fetch())[0];


#echo "<br>".$CCID."<br>";

$url = 'http://blitz.cs.niu.edu/CreditCard/';
$data = array(
'vendor' => 'VENDOR-3A',
'trans' => $CCID."-".date('m-d-y'),
'cc' => $formatedCCNum,
'name' => $name, 
'exp' => $ccExpSubmit, 
'amount' => $_GET['total']);

$options = array(
	'http' => array(
        'header' => array('Content-type: application/json', 'Accept: application/json'),
        'method' => 'POST',
        'content'=> json_encode($data)
 	)
);
$context = stream_context_create($options);
$result = file_get_contents($url, false, $context);
#echo($result);

session_unset();

echo '<b>ORDER SUBMITTED!</b>';


?>


</html>
