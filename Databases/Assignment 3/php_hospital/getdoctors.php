<?php
$hoscode = $_POST["hoscode"];

// Only if post is filled
if ($_POST != NULL) {
    $query = 'SELECT licensenum, firstname, lastname FROM doctor, hospital WHERE hoscode="' . $hoscode . '" AND hosworksat=hoscode';

    // Get Doctor info
    $result = mysqli_query($connection,$query);
    if (mysqli_num_rows($result) == 0) {
        echo "No results found";
    }
    while ($row = mysqli_fetch_assoc($result)) {
        echo '<tr>';
        echo '<td>' . $row["firstname"] . '</td>';
        echo '<td>' . $row["lastname"] . '</td>';
        echo '</tr>';
    }
    mysqli_free_result($result);
}
mysqli_close($connection);
?>