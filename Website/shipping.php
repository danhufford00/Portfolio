<html>
<style>
    table,
    th,
    td {
        border: 1px solid black;
        border-collapse: collapse;
        padding: 5px;
    }
</style>

<head>
    <title>Shipping Label</title>
</head>

<?php

require('dbconn.php');
$sql = "SELECT * FROM parts WHERE number = ?;";
$legacyStm = $legacydb->prepare($sql);

$orderId = $_POST['id'];

$sql = 'SELECT * FROM OrderContains WHERE orderId = ' . $orderId . ';';
$orderContainsStm = $newdb->prepare($sql);
$orderContainsStm->execute();
$order = $orderContainsStm->fetchAll(PDO::FETCH_ASSOC);

$total = 0;

foreach ($order as $row) {
    $legacyStm->execute(array($row["partNum"]));
    $row2 = $legacyStm->fetch(PDO::FETCH_ASSOC);
    $total += $row["qty"] * $row2["weight"];    
}

$sql = 'SELECT * FROM OrderPart WHERE id = ' . $orderId . ';';
$orderPartsStm = $newdb->prepare($sql);
$orderPartsStm->execute();

$orderInfo = $orderPartsStm->fetch(PDO::FETCH_ASSOC);

echo "<h1>SHIPPING LABEL</h1>";
echo "<hr>";
echo "<br>";

echo "<p>Order ID: ${orderInfo['id']}</p>";
echo "<p>Name: ${orderInfo['name']}</p>";
echo "<p>Address: ${orderInfo['shipAddr']}</p>";
echo "<p>Weight: $total</p>";
echo "<br>";

echo "<hr>";



?>


</html>
