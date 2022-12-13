USE assign2db;

-- Part 1 SQL Updates
SELECT * FROM hospital;
UPDATE hospital SET headdoc="GD56" WHERE hoscode="BBC";
UPDATE hospital SET headdocstartdate="2010-12-19" WHERE hoscode="BBC";
UPDATE hospital SET headdoc="SE66" WHERE hoscode="ABC";
UPDATE hospital SET headdocstartdate="2004-05-30" WHERE hoscode="ABC";
UPDATE hospital SET headdoc="YT67" WHERE hoscode="DDE";
UPDATE hospital SET headdocstartdate="2001-06-01" WHERE hoscode="DDE";
SELECT * FROM hospital;

-- Part 2 SQL Inserts
INSERT INTO doctor VALUES ("BL23", "Brandon", "Luu", "2022-10-09", "2002-05-23", "ABC", "Surgeon");
INSERT INTO patient VALUES ("123456789", "Keanu", "Reeves", "1964-09-02");
INSERT INTO looksafter VALUES ("BL23", "123456789");
INSERT INTO hospital VALUES ("XYZ", "Generic Hospital", "London", "ON", "1", "BL23", "2022-10-09");
SELECT * FROM doctor;
SELECT * FROM patient;
SELECT * FROM looksafter;
SELECT * FROM hospital;

-- Part 3 SQL Queries
-- Query 1
SELECT lastname FROM patient;
-- Query 2
SELECT DISTINCT lastname FROM patient;
-- Query 3
SELECT * FROM doctor GROUP BY lastname;
-- Query 4
SELECT hoscode, hosname FROM hospital WHERE numofbed>1500;
-- Query 5
SELECT firstname, lastname FROM doctor, hospital WHERE hosworksat=hoscode AND hosname="St. Joseph";
-- Query 6
SELECT firstname, lastname FROM patient WHERE lastname LIKE "G%";
-- Query 7
SELECT p.firstname, p.lastname FROM patient p, doctor d, looksafter l WHERE d.licensenum=l.licensenum AND p.ohipnum=l.ohipnum AND d.lastname="Webster";
-- Query 8
SELECT hosname, city, lastname FROM hospital, doctor WHERE licensenum=headdoc;
-- Query 9
SELECT SUM(numofbed) FROM hospital;
-- Query 10
SELECT p.firstname, p.lastname, d.firstname, d.lastname FROM doctor d, patient p, looksafter l, hospital AS h WHERE d.licensenum=l.licensenum AND h.headdoc=l.licensenum AND p.ohipnum=l.ohipnum;
-- Query 11
SELECT lastname, firstname, prov FROM doctor, hospital WHERE hosworksat=hoscode AND hosname="Victoria";
-- Query 12
SELECT firstname FROM doctor WHERE licensenum NOT IN (SELECT licensenum FROM looksafter);
-- Query 13
SELECT firstname, lastname, COUNT(l.licensenum), hosname FROM doctor d, hospital h, looksafter l WHERE d.hosworksat=h.hoscode AND l.licensenum=d.licensenum GROUP BY l.licensenum HAVING COUNT(l.licensenum)>1;
-- Query 14
SELECT firstname "Doctor First Name", lastname "Doctor Last Name", h1.hosname "Head of Hospital Name", h2.hosname "Works at Hospital Name" FROM doctor, hospital h1, hospital h2 WHERE h1.headdoc=licensenum AND h1.hoscode!=hosworksat AND h2.hoscode=hosworksat;
-- Query 15
-- Get all the patients' information who is currently being treated by a doctor that works at an Ontario (ON) hospital and put them under proper labels
SELECT p.ohipnum "Patient OHIP", p.firstname "Patient First Name", p.lastname "Patient Last Name", p.birthdate "Patient Birthday", d.firstname "Doctor First Name", d.lastname "Doctor Last Name", h.hosname "Hospital Name", h.prov "Province"
FROM looksafter l, patient p, doctor d, hospital h WHERE l.ohipnum=p.ohipnum AND l.licensenum=d.licensenum AND h.hoscode=d.hosworksat AND h.prov="ON";

-- Part 4 SQL Views/Deletes
CREATE VIEW birthpatdoc AS SELECT d.firstname dfirst, d.lastname dlast, d.birthdate dbirth, p.firstname pfirst, p.lastname plast, p.birthdate pbirth FROM doctor d, patient p, looksafter l WHERE d.licensenum=l.licensenum AND p.ohipnum=l.ohipnum;
SELECT * FROM birthpatdoc;
SELECT dlast, dbirth, plast, pbirth FROM birthpatdoc WHERE dbirth>pbirth;
SELECT * FROM patient;
SELECT * FROM looksafter;
DELETE FROM patient WHERE firstname="Keanu" AND lastname="Reeves";
SELECT * FROM patient WHERE firstname="Keanu" AND lastname="Reeves";
SELECT * FROM doctor;
DELETE FROM doctor WHERE firstname="Bernie";
SELECT * FROM doctor WHERE firstname="Bernie";
DELETE FROM doctor WHERE firstname="Brandon" AND lastname="Luu";
-- You cannot delete this doctor because they are the head doctor at the made up hospital
