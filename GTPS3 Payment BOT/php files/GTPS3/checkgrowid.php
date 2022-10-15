<?php
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
  die("registered|true\nprivateid|". $row["privateid"]);
  }
} else {
  die("registered|false\nprivateid|-1");
}

mysqli_close($link);


?>