<?php
$query = "SELECT * FROM doctor";

// Get doctor info
$result = mysqli_query($connection,$query);
if (!$result) {
    die("No results found");
}
while ($row = mysqli_fetch_assoc($result)) {
    echo '<option value="' . $row["licensenum"] . '">' . $row["licensenum"] . ' : ' . $row["firstname"] . ' ' . $row["lastname"] . '</option>';
}
mysqli_free_result($result);
?>