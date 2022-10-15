<?php
$PrivateGT = $_GET["privategt"]??null;
$RealGT = $_GET["realgt"]??null;

$link = mysqli_connect("localhost", "srmotion", "9ywn5#S99", "gtps3_paymentbot_db");
 
if($link === false){
    die("Connection Error");
}

$date = date('Y-m-d H:i:s');

$sql = "SELECT * FROM user_db WHERE growid = '$RealGT'";

$result = $link->query($sql);

if ($result->num_rows > 0) {
	//checking if anyone registered with this growID
  while($row = $result->fetch_assoc()) {
    $link->query("UPDATE `user_db` SET `privateid`='$PrivateGT' WHERE growid='$RealGT'");
	  die("upgraded!");
  }
} else {
if($PrivateGT == "" || 	$RealGT == "")
{
	die("use this api with right parameters");
}
$str = "INSERT INTO `user_db` (`date`, `growid`, `privateid`) VALUES ('$date', '$RealGT', '$PrivateGT');";
$result = $link->query($str);
echo("Successfuly registered to database!");
}

mysqli_close($link);


?>