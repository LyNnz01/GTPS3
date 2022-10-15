<?php
$name = strtolower($_GET["growid"]);
$deposit = $_GET["deposit"];
file_put_contents('logs.txt', "$name invalid deposit $deposit\r\n", FILE_APPEND);
echo("added invalid deposit " . $name . " wl: " . $deposit);

?>
