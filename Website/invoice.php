<html>
<style>
table, th, td {
  border: 1px solid black;
  border-collapse: collapse;
  padding: 5px;
}
</style>

<head><title>Invoice</title></head>

<?php
require('dbconn.php');

$orderId = $_POST['id'];

$sql = 'SELECT * FROM OrderPart WHERE id = '.$orderId.';';
$orderPartsStm = $newdb->prepare($sql);
$orderPartsStm->execute();

$sql = 'SELECT * FROM OrderContains WHERE orderId = '.$orderId.';';
$orderContainsStm = $newdb->prepare($sql);
$orderContainsStm->execute();

$orderInfo = $orderPartsStm->fetch(PDO::FETCH_ASSOC);

echo "
	<h1>INVOICE</h1>
	<b>Bill to: </b>
	<p>${orderInfo['name']} <br>
           ${orderInfo['shipAddr']}</p>   
";

$sql = "SELECT * FROM parts WHERE number = ?;";
$legacyStm = $legacydb->prepare($sql);

$result1 = $orderContainsStm->fetchAll(PDO::FETCH_ASSOC);


echo "<table> <thead><th>Number</th><th>QTY</th><th>Description</th><th>Price</th></thead>";
foreach($result1 as $row){

	$legacyStm->execute(array($row['partNum']));
	$row2 = $legacyStm->fetch(PDO::FETCH_ASSOC);

	echo "<tr>
	      <td>${row['partNum']}</td>
	      <td>${row['qty']}</td>
	      <td>${row2['description']}</td>
	      <td>${row2['price']}</td>	
	      </tr>";

}
echo "</table>
      <h4>TOTAL: \$${orderInfo['total']}</h4>
";



?>


</html>
