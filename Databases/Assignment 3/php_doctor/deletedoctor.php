<?php
// Programmer Name: 51

$licensenum = $_POST["licensenum"];

// Delete only if post is filled
if ($_POST != NULL) {
    try {
        // Validity
        if ($licensenum == NULL || $licensenum == "") throw new Exception("The license number cannot be blank.");

        $query = 'SELECT * FROM doctor WHERE licensenum="' . $licensenum . '"';
        $result = mysqli_query($connection,$query);
        if (mysqli_num_rows($result) == 0) throw new Exception("A doctor with license number " . $licensenum . " does not exist.");
        mysqli_free_result($result);
        
        // Do deletion
        $query = 'DELETE FROM doctor WHERE licensenum="' . $licensenum . '"';

        $result = mysqli_query($connection,$query);
        if (!$result) {
            echo '<div class="alert alert-danger" role="alert">Invalid license number entered, or the doctor is bound to a patient/hospital.</div>';
        }
        else {
            echo '<div class="alert alert-success" role="alert">Doctor ' . $licensenum . ' successfully deleted.</div>';
        }
    }
    // Failed
    catch (Exception $e) {
        echo '<div class="alert alert-danger" role="alert">' . $e->getMessage() . '</div>';
    }
}
mysqli_close($connection);
?>