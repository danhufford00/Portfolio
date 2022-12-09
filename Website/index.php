<html>

<header><title>Product System</title></header>

<h1>Welcome to Discount Amazon: </h1>
	<form action="orderSubmit.php" method="POST">
	<input type='submit' name='orderSubmit' value='ORDER NOW!'>
<?php
require('dbconn.php');

$sql = 'SELECT * FROM parts;';

$stm = $legacydb->prepare($sql);
$stm->execute();
$products = $stm->fetchAll( PDO::FETCH_ASSOC );

$sql = 'SELECT * FROM Inventory WHERE partNum = ?;';
$inv = $newdb->prepare($sql);

foreach ($products as $row){
	$qty = 0;

	$inv->execute(array($row['number']));
	if ($inv->rowCount() != 0){
		$r = $inv->fetch( PDO::FETCH_ASSOC );
		$qty = $r['qty'];
	}else{
		continue;
	}

	if ($qty > 0){

		echo ' 	<h3>'.$row['description'].'</h3>
			<p><img src="'.$row['pictureURL'].'"></p>
			<p> <b>Price: </b>'.$row['price'].' </p>
			<p>Available QTY: '.$qty.'</p>
			<h5>Order QTY: <input type="number" name="'.$row['number'].'"></h5>
		';
	
	}

}




?>
	<input type='submit' name='orderSubmit' value='ORDER NOW!'>
	</form>
</html>
