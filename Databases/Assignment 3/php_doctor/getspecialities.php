<?php
// Programmer Name: 51

// Get specialities
$query = "SELECT DISTINCT speciality FROM doctor";
$result = mysqli_query($connection,$query);
if (!$result) {
    die("No results found");
}
while ($row = mysqli_fetch_assoc($result)) {
     echo '<option value="' . $row["speciality"] . '">' . $row["speciality"] . '</option>';
     // <option value="None">None</option>
}
mysqli_free_result($result);
?>