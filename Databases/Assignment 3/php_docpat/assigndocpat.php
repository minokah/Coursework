<?php
// Programmer Name: 51

$licensenum = $_POST["licensenum"];
$ohipnum = $_POST["ohipnum"];

// Delete only if post is filled
if ($_POST != NULL) {
    try {
        // Check validity
        if ($licensenum == NULL || $licensenum == "") throw new Exception("The license number cannot be blank.");
        if ($ohipnum == NULL || $ohipnum == "") throw new Exception("The OHIP number cannot be blank.");

        // Check if entries is in looksafter table already
        $query = 'SELECT * FROM looksafter WHERE licensenum="' . $licensenum . '" AND ohipnum="' . $ohipnum . '"';
        $result = mysqli_query($connection,$query);

        // Throw exception if exists
        if (mysqli_num_rows($result) != 0) throw new Exception("This doctor has already been assigned to this patient.");
        
        $query = 'INSERT INTO looksafter (licensenum, ohipnum) VALUES ("' . $licensenum . '", "' . $ohipnum . '")';

        // Attempt inesertion
        $result = mysqli_query($connection,$query);
        if (!$result) {
            // Invalid for either or
            echo '<div class="alert alert-danger" role="alert">Invalid license number or OHIP number entered.</div>';
        }
        else {
            // Good
            echo '<div class="alert alert-success" role="alert">Doctor ' . $licensenum . ' successfully assigned to patient ' . $ohipnum . '!</div>';
        }
    }
    catch (Exception $e) {
        // Failed for whatever reason
        echo '<div class="alert alert-danger" role="alert">' . $e->getMessage() . '</div>';
    }
}
mysqli_close($connection);
?>