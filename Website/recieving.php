<html>
<style>
table, th, td {
  border: 1px solid black;
  border-collapse: collapse;
  padding: 5px;
}
</style>
<head><title>Receiving Desk</title></head>

<p><a href="http://students.cs.niu.edu/~z1861935/467Project/employee.php">Go Back</a><p>

<?php
require("env.php");
require("dbconn.php");

#here is where we proccess new inventory

if(isset($_POST['SubmitInv'])){ #only prints
	$sql = "SELECT * FROM Inventory WHERE partNum = ?;";
	$stm = $newdb->prepare($sql); #prepares a PDOStament that will be used to fetch current inventory amounts
	
	foreach($_POST as $key => $value){
		if(!is_numeric($key) || !is_numeric($value)){ #checks to make sure only numeric values are present
			break;
		}
		if($value == 0){ #no need to update if value is 0
			continue;
		}
		$stm->execute(array($key));
		if ($row = $stm->fetch(PDO::FETCH_ASSOC)){ #this block updates a qty if there is already stored inventory
			$newqty = $value + $row['qty'];
			$sql = "UPDATE Inventory SET qty = ? WHERE partNum = ".$key.";";
			$update = $newdb->prepare($sql);
			$update->execute(array($newqty));
		}else{ #if its a new item in inventory it gets added
			$sql = "INSERT INTO Inventory VALUE (?, ?);";
			$insert = $newdb->prepare($sql);
			$insert->execute(array($key, $value));
		}
	}

	echo "<p>Inventory Sucessfully Updated!</p>"; #success message
}

$sql = "SELECT * FROM parts;";

$res = $legacydb->query($sql);

$result = $res->fetchAll(PDO::FETCH_ASSOC);
?>
<form action="<?php echo $mainDir;?>recieving.php" method="POST">
	<table>
	<thead>
		<th>Part Number</th><th>Part Discription</th><th>Amount Recieved</th>
	</thead>
<?php
foreach($result as $row){
	echo '<tr>';
        echo '<td>';
        echo $row['number'];
        echo '</td>';
        echo '<td>';
        echo $row['description'];
	echo '</td>';
	echo '<td>';
	echo '<input type="textbox" name='.$row['number'].' value="0">';
	echo '</td>';
	echo '</tr>';
}
?>
	</table>
	<br>
	<input type="submit" name="SubmitInv" value="SUBMIT">
</form>
</html>
