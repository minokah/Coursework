<?php
// Programmer Name: 51
$hoscode = $_POST["hoscode"];
$numofbed = $_POST["numofbed"];

// Only if post is filled
if ($_POST != NULL) {
    $query = 'UPDATE hospital SET numofbed="' . $numofbed . '" WHERE hoscode="' . $hoscode . '"';

    $result = mysqli_query($connection,$query);
    
    // Set the bed count
    if (!$result) {
        echo '<div class="alert alert-danger" role="alert">Invalid bed number entered. Please check your input.</div>';
    }
    else {
        echo '<div class="alert alert-success" role="alert">Bed count successfully set to ' . $numofbed . '!</div>';
    }
}
?>