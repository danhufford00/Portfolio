<html>
<style>
table, th, td {
  border: 1px solid black;
  border-collapse: collapse;
  padding: 5px;
}
</style>
<head><title>Warehouse Desk</title></head>
<p><a href="http://students.cs.niu.edu/~z1861935/467Project/employee.php">Go Back</a><p>
<?php
require("dbconn.php");
	echo '<form action="fulfillOrder.php" method="POST">';
	$sql = 'SELECT * FROM OrderPart;';
	$stm = $newdb->prepare($sql);
	$stm->execute();
	$parts = $stm->fetchAll(PDO::FETCH_ASSOC);

	echo '<table><thead><th>ID</th><th>Name</th><th>Address</th><th>Email</th><th>CCNum</th><th>CCExp</th><th>Status</th><th>Total</th><th>Fulfill</th></thead>';

	foreach($parts as $row){
		echo '<tr>';
		echo '<td>'.$row["id"].'</td>';
		echo '<td>'.$row["name"].'</td>';
		echo '<td>'.$row["shipAddr"].'</td>';
		echo '<td>'.$row["email"].'</td>';
		echo '<td>'.$row["CCNum"].'</td>';
		echo '<td>'.$row["CCExp"].'</td>';
		echo '<td>'.$row["isComplete"].'</td>';
		echo '<td>'.$row["total"].'</td>';
		echo '<td><input type="radio" name="submit" value="';
		echo $row['id'];
		echo '"</td></tr>';
	}
	echo '</table><input type="submit" name="sub" value="Fulfill Order">';

?>
</html>
