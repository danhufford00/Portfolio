<html>
<?php
require('env.php');
if (isset($_POST["empType"])){
	if($_POST["empType"] ==	"warehouse"){
		header("Location: ".$mainDir."warehouse.php");
	}else if($_POST["empType"] == "recieving"){
		header("Location: ".$mainDir."recieving.php");
	}else{
		header("Location: ".$mainDir."admin.php");
	}
}
echo "<br>";
?>
<head><title>Employee Login</title></head>
<h1>Enter Job Location:</h1>
<form action="<?php echo $mainDir?>employee.php" method="POST">
	<input type="radio" id="warehouse" name="empType" value="warehouse">
  	<label for="male">Warehouse Worker</label><br>
  	<input type="radio" id="revieving" name="empType" value="recieving">
  	<label for="female">Recieving Desk</label><br>
	<input type="radio" id="admin" name="empType" value="admin">
	<label for="other">Administrator</label>
	<br>
	<input type="submit" value="Submit">
</html>
