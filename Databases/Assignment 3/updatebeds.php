<html>

<head>
    <title>Hospital Database</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css" rel="stylesheet"
        integrity="sha384-Zenh87qX5JnK2Jl0vWa8Ck2rdkQ2Bzep5IDxbcnCeuOxjzrPF/et3URy9Bv1WTRi" crossorigin="anonymous">
    <link href="styles.css" rel="stylesheet">
</head>

<body>
    <?php
        include 'connectdb.php';
    ?>
    <div id="header" class="center">
        <br>
        <a href="index.html" id="homelink">
            <h1>Hospital Database</h1>
        </a>
        <p>Look and edit Doctors, Patients, and Hospital information.</p>
    </div>

    <div class="container">
        <div class="row mt-4 mb-2">
            <nav aria-label="breadcrumb">
                <ol class="breadcrumb">
                    <li class="breadcrumb-item"><a href="index.html">Home</a></li>
                    <li class="breadcrumb-item active" aria-current="page">Update Bed Count</li>
                </ol>
            </nav>
            <?php
                include 'php_hospital/updatebeds.php';
            ?>
            <h2>Update Bed Count</h2>
        </div>
        <form action method="post">
            <div class="row">
                <p>Select a hospital and input the new bed count.</p>
                <div class="row">
                <h5>Select Hospital</h5>
                <div class="row">
                    <div class="col-3">
                        <select class="form-select mb-3" name="hoscode" aria-label="Hospital">
                            <?php
                                    include 'php_hospital/gethospitals.php';
                                ?>
                        </select>
                        <div class="form-floating mb-3">
                            <input type="text" class="form-control" id="numofbed" name="numofbed">
                            <label for="numofbed">New # of Beds</label>
                        </div>
                    </div>
                </div>
                <div class="row mt-4">
                    <div class="col-6">
                    <button type="button" class="btn btn-primary" onclick="this.form.submit()">Update Bed Count</button>
                    </div>
                </div>
            </div>
        </form>
    </div>
    <br>
    <p class="center">😺 Made by a student whose student number ends in 51</p>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/js/bootstrap.bundle.min.js"
        integrity="sha384-OERcA2EqjJCMA+/3y+gxIOqMEjwtxJY7qPCqsdltbNJuaOe923+mo//f6V8Qbsw3"
        crossorigin="anonymous"></script>
</body>

</html>