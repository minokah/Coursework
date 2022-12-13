<?php
// Programmer Name: 51

$hoscode = $_POST["hoscode"];

// Only if post is filled
if ($_POST != NULL) {
    $query = 'SELECT * FROM hospital, doctor WHERE hoscode="' . $hoscode . '" AND headdoc=licensenum';

    $result = mysqli_query($connection,$query);
    
    while ($row = mysqli_fetch_assoc($result)) {
        echo '<tr>';
        echo '<td>' . $row["hoscode"] . '</td>';
        echo '<td>' . $row["hosname"] . '</td>';
        echo '<td>' . $row["city"] . '</td>';
        echo '<td>' . $row["prov"] . '</td>';
        echo '<td>' . $row["numofbed"] . '</td>';
        echo '<td>' . $row["firstname"] . ' ' . $row["lastname"] . '</td>';
        echo '</tr>';
    }
    mysqli_free_result($result);
}
?>