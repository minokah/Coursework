<?php
// Programmer Name: 51

$firstname = $_POST["firstname"];
$lastname = $_POST["lastname"];
$birthdate = $_POST["birthdate"];
$licensenum = $_POST["licensenum"];
$licensedate = $_POST["licensedate"];
$speciality = $_POST["speciality"];
$hosworksat = $_POST["hosworksat"];

// Insert only if post is filled
if ($_POST != NULL) {
    try {

        // Check validity
        if ($firstname == NULL || $firstname == "") throw new Exception("The first name cannot be blank.");
        if ($lastname == NULL || $lastname == "") throw new Exception("The last name cannot be blank.");
        if ($birthdate == NULL || $birthdate == "") throw new Exception("The birthdate cannot be blank.");
        if ($licensenum == NULL || $licensenum == "") throw new Exception("The license number cannot be blank.");
        if ($licensedate == NULL || $licensedate == "") throw new Exception("The license date cannot be blank.");
        if ($speciality == NULL || $speciality == "") throw new Exception("The speciality cannot be blank.");
        if ($hosworksat == NULL || $hosworksat == "") throw new Exception("The hospital ID cannot be blank.");

        $query = 'SELECT * FROM doctor WHERE licensenum="' . $licensenum . '"';
        $result = mysqli_query($connection,$query);
        if (mysqli_num_rows($result) != 0) throw new Exception("A doctor with License Number " . $licensenum . " already exists.");
        mysqli_free_result($result);

        $query = 'SELECT * FROM hospital WHERE hoscode="' . $hosworksat . '"';
        $result = mysqli_query($connection,$query);
        if (mysqli_num_rows($result) == 0) throw new Exception("Hospital ID " . $hosworksat . " does not exist.");
        mysqli_free_result($result);
        
        $query = 'INSERT INTO doctor (licensenum, firstname, lastname, licensedate, birthdate, hosworksat, speciality) VALUES ("' . $licensenum . '", "' . $firstname . '", "' . $lastname . '", "' . $licensedate . '", "' . $birthdate . '", "' . $hosworksat . '", "' . $speciality . '")';

        // Attempt insertion query
        $result = mysqli_query($connection,$query);
        if (!$result) {
            echo '<div class="alert alert-danger" role="alert">Invalid doctor information entered. Please check your input.</div>';
        }
        else {
            echo '<div class="alert alert-success" role="alert">Doctor ' . $firstname . ' ' . $lastname . ' successfully added!</div>';
        }
    }
    catch (Exception $e) {
        echo '<div class="alert alert-danger" role="alert">' . $e->getMessage() . '</div>';
    }
}
?>