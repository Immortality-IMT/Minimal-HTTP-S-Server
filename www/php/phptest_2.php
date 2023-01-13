<html>
<body>

<?php
echo "<p>.Server Method.</p>";
echo $_SERVER['REQUEST_METHOD'];
echo "<p>.POST.</p>";
print_r($_POST);
echo "<p>GET.</p>";
print_r($_GET);
echo "<p>Return</p>";
?>

Welcome <?php echo $_POST["name"]; ?><br />
Your email address is: <?php echo $_POST["email"]; ?><br />

<form action="phptest_2.php" method="POST">
Name: <input type="text" name="name"><br>
E-mail: <input type="text" name="email"><br>
<input type="submit">
</form>


<?php
$servername = "localhost";
$username = "root";
$password = "<insert password here>";
$dbname = "gochat";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT * FROM chatlog";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
        echo "id: " . $row["message_id"]. " - message: " . $row["message"]. " - date_time: " . $row["message_date"]. " - account: " . $row["account"]. " - User id: " . $row["user_id"]. "<br>";
    }
} else {
    echo "0 results";
}
$conn->close();
?>

</body>
</html>
