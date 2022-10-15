<?php
$wl = $_GET["wl"];
$growid = $_GET["growid"];


$ch = curl_init();
curl_setopt($ch, CURLOPT_URL,'https://privategts1.eu/deposit.php?growid='. $growid . '&deposit=' . $wl);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);

$response = curl_exec($ch);
curl_close($ch); 

echo $response;

?>