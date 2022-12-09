<html>

<head>
    <title>Order Submission</title>

    <meta charset="UTF-8">



</head>

<body>

    <h1>Order Submission</h1>
    <p><a href="http://students.cs.niu.edu/~z1861935/467Project/">Go Back</a></p>
    <hr>

<?php

require('dbconn.php');
    session_start();
    session_unset();
    $sql = 'SELECT * FROM parts;';
    $stm = $legacydb->prepare($sql);
    $stm->execute();

    $res = $stm->fetchAll(PDO::FETCH_ASSOC);

    $productPrice = 0;
    $totalWeight = 0;
    foreach ($res as $row) {

        if (isset($_POST[$row["number"]]) && $_POST[$row["number"]] > 0) {
            $_SESSION["part".$row["number"]] = $_POST[$row["number"]]; #keep track of order

            $productPrice += $_POST[$row["number"]] * $row['price'];
            $totalWeight += $_POST[$row["number"]] * $row['weight'];
        }
    }

    $shipPrice = 0;

    $sql = "SELECT * FROM Ship;";
    $shipStm = $newdb->prepare($sql);

    $shipStm->execute();

    $res = $shipStm->fetchAll( PDO::FETCH_ASSOC );
    
    $shipPrice = 0;
    foreach($res as $row){

	    if($row['bracket'] <= $totalWeight){
	    	$shipPrice = $row['price'];
	    }
    
    }
    $totalPrice = $productPrice + $shipPrice;
    echo '<p>Product Price: '.$productPrice.' Shipping price: '.$shipPrice.'</p>
          <p><b>Total Price = '.$totalPrice.' </b></p>';




    ?>
    <hr>
    <form action="orderConfirmation.php" method="GET">
        Enter your name: <input type="text" name="name"> <br>
        Enter your e-mail address: <input type="text" name="email"><br>
        Enter your address: <input type="text" name="address"> <br>
        Enter your 16 digit Credit Card Number: <input type="number" name="creditCard"> <br>
        Enter Credit Card Expiration Date: <input type="month" name="expDate" min="2021-1" max="2030-1"> <br>
	<br>
	<?php echo '<input type="hidden" name="total" value="'.$totalPrice.'">';?>
	<input type="submit" name="completeOrder" value="Submit Order">
	<input type="hidden" name="weight" value="<?php echo $totalWeight?>">
        <br>

    </form>

    <br>
    <hr>

<?php

    /*
    if(isset($_GET['completeOrder']))
    {
        echo ""
    }

    /*
    $url = 'http://blitz.cs.niu.edu/CreditCard/';
    $data = array(
    	'vendor' => 'VE001-99',
        'trans' => '907-987654321-296',
    	'cc' => '6011 1234 4321 1234',
    	'name' => 'John Doe', 
    	'exp' => '12/2024', 
    	'amount' => '654.32');

    $options = array(
        'http' => array(
        'header' => array('Content-type: application/json', 'Accept: application/json'),
        'method' => 'POST',
        'content'=> json_encode($data)
        )
    );

    $context  = stream_context_create($options);
    $result = file_get_contents($url, false, $context);
    echo($result);
 */
    ?>


</body>


</html>
