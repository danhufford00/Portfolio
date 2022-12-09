<html>
<style>
table, th, td {
  border: 1px solid black;
  border-collapse: collapse;
  padding: 5px;
}
</style>
<header><title>Admin Control</title></header>

<p><a href="http://students.cs.niu.edu/~z1861935/467Project/employee.php">Go Back</a><p>
<?php

require("dbconn.php");
if(isset($_POST['adminSet']) && $_POST['adminSet'] == 'seeOrders')
{
	echo '<p>Search Current Orders</p>';

	echo '	<form action="admin.php" method="POST">

			<input type="radio" id="authorized" name="status" value="authorized">
  			<label for="authorized">Order Authorized</label><br>

			<input type="radio" id="shipped" name="status" value="shipped">
			<label for="shipped">Order Shipped</label><br>

			<input type="radio" id="nA" name="status" value="nA">
			<label for="nA">N/A</label><br>

			<p>Date Range</p>
			<input type="date" name="expDateLower" min="2021-1" max="2030-1"> <br>
			<label for="nA">Minimum Date Range</label><br>
			<input type="date" name="expDateUpper" min="2021-1" max="2030-1"> <br>
			<label for="nA">Maximum Date Range</label><br>

			<p>Price Range</p>
			<input type="number" name="MinWeightRange" value="MinWeight"> <br>
			<label for="nA">Minimum Price Range</label><br>
			<input type="number" name="MaxWeightRange" value="MaxWeight"> <br>			
			<label for="nA">Maximum Price Range</label><br>



			<br>
			<input type="submit" name="adminSearch" value="Search"> <br>';
}
if(isset($_POST['adminSearch']))
{
	$sql = "SELECT * FROM OrderPart WHERE id >= 0";

	if(isset($_POST['status']) && $_POST['status'] == "shipped")
	{
		$sql = $sql . " AND isComplete = 1";
	}
	else if (isset($_POST['status']) && $_POST['status'] == "authorized") 
	{
		$sql = $sql . " AND isComplete = 0";
	}

	if($_POST['expDateLower'] != "")
	{
		$sql = $sql . " AND dateSub >= '${_POST['expDateLower']}'";

	}

	if ($_POST['expDateUpper'] != "") 
	{
		$sql = $sql . " AND dateSub <= '${_POST['expDateUpper']}'";
	}
	

	if ($_POST['MaxWeightRange'] != "")
	{
		$sql = $sql . " AND total <= ${_POST['MaxWeightRange']}";
	}

	if ($_POST['MinWeightRange'] != "") 
	{
		$sql = $sql . " AND total >= ${_POST['MinWeightRange']}";
	}

	$sql = $sql . ";";

	#echo "<br>$sql<br>";



	$stm = $newdb->prepare($sql);
	$stm->execute();
	$result = $stm->fetchAll(PDO::FETCH_ASSOC);

	echo '<table><thead><th>ID</th><th>Name</th><th>Address</th><th>Email</th><th>CCNum</th><th>CCExp</th><th>Status</th><th>Weight</th><th>Submission Date</th><th>Total</th></thead>';
	foreach($result as $row)
	{
		echo '<tr>';
		echo '<td>' . $row["id"] . '</td>';
		echo '<td>' . $row["name"] . '</td>';
		echo '<td>' . $row["shipAddr"] . '</td>';
		echo '<td>' . $row["email"] . '</td>';
		echo '<td>' . $row["CCNum"] . '</td>';
		echo '<td>' . $row["CCExp"] . '</td>';
		echo '<td>' . $row["isComplete"] . '</td>';
		echo '<td>' . $row["weight"] . '</td>';
		echo '<td>' . $row["dateSub"] . '</td>';
		echo '<td>' . $row["total"] . '</td>';
	}
}




if(isset($_POST['updateDB']) && $_POST['updateDB'] == 'Update'){

	$sql = "SELECT bracket FROM Ship;";
	$stm = $newdb->prepare($sql);
	$stm->execute();
	$result = $stm->fetchAll(PDO::FETCH_ASSOC);
	
	$doesExist = false;
	foreach ($result as $row){

		if($row['bracket'] == $_POST['bracketWeight']){
			$sql = 'UPDATE Ship SET price = ? WHERE bracket = '.$row['bracket'].';';

			$stm = $newdb->prepare($sql);
			$stm->execute(array($_POST['bracketCost']));
			$doesExist = true;
		}
	
	}
	if (!$doesExist){
		$sql = 'INSERT INTO Ship VALUES (?, ?);';

		$stm = $newdb->prepare($sql);
		$stm->execute(array($_POST['bracketWeight'], $_POST['bracketCost']));
	}

}

if(isset($_POST['deleteDB']) && $_POST['deleteDB'] == 'Delete'){

	$sql = 'DELETE FROM Ship WHERE bracket = ?;';
	$stm = $newdb->prepare($sql);
	$stm->execute(array($_POST['deleteValue']));

}

if (isset($_POST['adminSet']))
{
	if($_POST['adminSet'] == 'bracket')
	{

		$sql = "SELECT * FROM Ship;";
		$stm = $newdb->prepare($sql);

		$stm->execute();

		$result = $stm->fetchAll(PDO::FETCH_ASSOC);
		echo '<table>';
		echo '<thead><th>Bracket (Weight in IBS)</th><th>Price($)</th></thead>';
		foreach($result as $row){
			echo '<tr>';
				echo '<td>';
				echo $row['bracket'];
				echo '</td>';
				echo '<td>';
				echo $row['price'];
			echo '</td>';
			echo '</tr>';
		}
		echo '</table>';

		echo '<p>Update Price Brackets:</p>';

		echo '	<form action="admin.php" method="POST">

				Bracket Weight: <input type="number" name="bracketWeight" value="" style="100px"/>
				Bracket Cost: <input type="number" name="bracketCost" value="" style="100px"/>
				<input type="submit" name="updateDB" value="Update">
				<br>

				<p>Delete Weight Bracket:</p>';
		echo '<select name="deleteValue">';
			foreach($result as $row){

				echo '<option value="'.$row['bracket'].'">'.$row['bracket'].'</option>';
			
			}

			echo '
			</select>
			<br>
				<p><input type="submit" name="deleteDB" value="Delete"></p>
			</form>';

		echo '<form action>';
	}
}

?>

<form action="admin.php" method="POST">
	<input type="radio" id="bracket" name="adminSet" value="bracket">
  	<label for="bracket">Set Weight Brackets</label><br>
  	<input type="radio" id="seeOrders" name="adminSet" value="seeOrders">
  	<label for="seeOrders">View Current Orders</label><br>
	<br>
	<input type="submit" value="Submit">

</form>
</html>
