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
                    <li class="breadcrumb-item active" aria-current="page">View Hospitals</li>
                </ol>
            </nav>
            <h2>View Hospitals</h2>
        </div>
        <form action method="post">
            <div class="row">
                <div class="col-2">
                    <button type="button" class="btn btn-primary" onclick="this.form.submit()">Search for Hospital</button>
                </div>
                <div class="col-2">
                    <h5>Filter By</h5>
                    <select class="form-select" name="hoscode" aria-label="Hospital">
                        <?php
                                include 'php_hospital/gethospitals.php';
                            ?>
                    </select>
                </div>
            </div>
        </form>
        <div class="row mt-4 mb-2">
            <h4>Hospital Information</h4>
        </div>
        <div class="row">
            <ul class="list-group">
                <table class="table">
                    <tr>
                        <th>Code</th>
                        <th>Name</th>
                        <th>City</th>
                        <th>Province</th>
                        <th># of Beds</th>
                        <th>Head Doctor</th>
                    </tr>
                    <?php
                            include 'php_hospital/gethospitaldata.php';
                        ?>
                </table>
            </ul>
        </div>
        <div class="row mt-4 mb-2">
            <h4>Doctors that Work Here</h4>
        </div>
        <div class="row">
            <div class="col-4">
                <ul class="list-group">
                    <table class="table">
                        <tr>
                            <th>First Name</th>
                            <th>Last Name</th>
                        </tr>
                        <?php
                                include 'php_hospital/getdoctors.php';
                            ?>
                    </table>
                </ul>
            </div>
        </div>
    </div>
    <br>
    <p class="center">😺 Made by a student whose student number ends in 51</p>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/js/bootstrap.bundle.min.js"
        integrity="sha384-OERcA2EqjJCMA+/3y+gxIOqMEjwtxJY7qPCqsdltbNJuaOe923+mo//f6V8Qbsw3"
        crossorigin="anonymous"></script>
</body>

</html>