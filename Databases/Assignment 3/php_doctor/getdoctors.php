<?php
// Programmer Name: 51

$filterSpeciality = $_POST["filterSpeciality"];
$radioSort = $_POST["radioSort"];
$radioOrder = $_POST["radioOrder"];

// Only if post is filled
if ($_POST != NULL) {
    $query = "SELECT * FROM doctor";

    // Tack on some filters
    if ($filterSpeciality != "None") {
        $query = $query . ' WHERE speciality="' . $filterSpeciality . '"';
    }

    $query = $query . ' ORDER BY ' . $radioSort . ' ' . $radioOrder;
}
else {
    // default query
    $query = "SELECT * FROM doctor ORDER BY lastname ASC";
}

// Get doctor info
$result = mysqli_query($connection,$query);
if (!$result) {
    die("No results found");
}
while ($row = mysqli_fetch_assoc($result)) {
    echo "<tr>";
    echo "<td>" . $row["licensenum"] . "</td>";
    echo "<td>" . $row["firstname"] . "</td>";
    echo "<td>" . $row["lastname"] . "</td>";
    echo "<td>" . $row["licensedate"] . "</td>";
    echo "<td>" . $row["birthdate"] . "</td>";
    echo "<td>" . $row["hosworksat"] . "</td>";
    echo "<td>" . $row["speciality"] . "</td>";
    echo "</tr>";
}
mysqli_free_result($result);
mysqli_close($connection);
?>