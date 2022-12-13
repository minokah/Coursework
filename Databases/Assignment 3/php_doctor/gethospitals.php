<?php
// Programmer Name: 51

// Get hospitals
$query = "SELECT DISTINCT hoscode, hosname FROM hospital";

$result = mysqli_query($connection,$query);

while ($row = mysqli_fetch_assoc($result)) {
    echo '<option value="' . $row["hoscode"] . '">' . $row["hoscode"] . ' : ' . $row["hosname"] . '</option>';
}
mysqli_free_result($result);
mysqli_close($connection);
?>