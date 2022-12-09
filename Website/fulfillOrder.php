<html>
<style>
table, th, td {
  border: 1px solid black;
  border-collapse: collapse;
  padding: 5px;
}
</style>
<?php

require('dbconn.php');

if(isset($_POST['shipIt'])){
	
	$sql = "UPDATE OrderPart SET isComplete = 1 WHERE id = ${_POST['id']};";
	$stm = $newdb->prepare($sql);
	if($stm->execute()){
		echo "Order Successfully Set as Shipped!";
	}else{
		echo "Order Failed!";
	}	
	

}else{
	$sql = 'SELECT * FROM OrderContains WHERE orderId = '.$_POST['submit'].';';
	$stm = $newdb->prepare($sql);
	$stm->execute();
	$order = $stm->fetchAll(PDO::FETCH_ASSOC);

	echo '<table><thead><th>Part Num</th><th>Quantity</th></thead>';

	foreach($order as $row){
		echo '<tr>';
		echo '<td>'.$row["partNum"].'</td>';
		echo '<td>'.$row["qty"].'</td></tr>';
	}
	echo '</table>';
	echo '<form action="invoice.php" method="POST">';
	echo  '<input type="submit" name="invoice" value= "Print invoice">';
	echo  '<input type="hidden" name="id" value= '.$_POST['submit'].'>';

	echo "</form>";

	echo '<form action="shipping.php" method="POST">';
	echo  '<input type="submit" name="shipping" value= "Print shipping">';
	echo  '<input type="hidden" name="id" value='.$_POST['submit'].'>';
	echo "</form>";	

	echo '<form action="fulfillOrder.php" method="POST">';
	echo  '<input type="submit" name="shipIt" value= "Mark Order As Shipped">';
	echo  '<input type="hidden" name="id" value='.$_POST['submit'].'>';
	echo "</form>";	
}
?>
</html>
