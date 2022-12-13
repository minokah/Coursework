<?php
// Programmer Name: 51

$selectDoctor = $_POST["selectDoctor"];

// Query only if post is filled
if ($_POST != NULL) {
    $query = 'SELECT p.ohipnum, p.firstname, p.lastname, p.birthdate FROM patient p, looksafter l WHERE l.licensenum="' . $selectDoctor . '" AND p.ohipnum=l.ohipnum';

    // Get looksafter info
    $result = mysqli_query($connection,$query);
    if (mysqli_num_rows($result) == 0) {
        echo "No results found";
    }
    while ($row = mysqli_fetch_assoc($result)) {
        echo "<tr>";
        echo "<td>" . $row["ohipnum"] . "</td>";
        echo "<td>" . $row["firstname"] . "</td>";
        echo "<td>" . $row["lastname"] . "</td>";
        echo "</tr>";
    }
    mysqli_free_result($result);
}
else {
    echo "No results found";
}

mysqli_close($connection);
?>