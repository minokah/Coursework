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
                    <li class="breadcrumb-item active" aria-current="page">Insert Doctor</li>
                </ol>
            </nav>
            <?php
                include 'php_doctor/insertdoctor.php';
            ?>
            <h2>Insert Doctor</h2>
        </div>
        <form action method="post">
            <div class="row">
                <p>Input the doctor's information below.</p>
                <div class="row">
                    <h5>Doctor Name</h5>
                    <div class="col-3">
                        <div class="form-floating mb-3">
                            <input type="text" class="form-control" id="firstname" name="firstname">
                            <label for="firstname">First Name</label>
                        </div>
                    </div>
                    <div class="col-3">
                        <div class="form-floating mb-3">
                            <input type="text" class="form-control" id="lastname" name="lastname">
                            <label for="lastname">Last Name</label>
                        </div>
                    </div>
                    <div class="col-3">
                        <div class="form-floating mb-3">
                            <input type="text" class="form-control" id="birthdate" name="birthdate">
                            <label for="birthdate">Birth Date (ie. 2022-11-24)</label>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <h5>License Information</h5>
                    <div class="col-3">
                        <div class="form-floating mb-3">
                            <input type="text" class="form-control" id="licensenum" name="licensenum">
                            <label for="licensenum">License Number</label>
                        </div>
                    </div>
                    <div class="col-3">
                        <div class="form-floating mb-3">
                            <input type="text" class="form-control" id="licensedate" name="licensedate">
                            <label for="licensedate">License Date (ie. 2022-11-24)</label>
                        </div>
                    </div>
                    <div class="col-3">
                        <div class="form-floating mb-3">
                            <input type="text" class="form-control" id="speciality" name="speciality">
                            <label for="speciality">Speciality</label>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <h5>Hospital Selection</h5>
                    <div class="col-3">
                        <select class="form-select" name="hosworksat" aria-label="Doctor">
                            <?php
                                    include 'php_doctor/gethospitals.php';
                                ?>
                        </select>
                    </div>
                </div>
                <div class="col-6 mt-4">
                    <button type="button" class="btn btn-primary" onclick="this.form.submit()">Insert New
                        Doctor</button>
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