-- ---------------------------------
-- SCRIPT 1

-- Set up the database

SHOW DATABASES;
DROP DATABASE IF EXISTS assign2db;
CREATE DATABASE assign2db;
USE assign2db; 


-- Create the tables for the database

SHOW TABLES;

CREATE TABLE doctor(licensenum CHAR(4) NOT NULL, firstname VARCHAR(20) NOT NULL, lastname VARCHAR(20) NOT NULL, licensedate DATE NOT NULL, birthdate DATE NOT NULL, hosworksat CHAR(3), speciality VARCHAR(30), PRIMARY KEY(licensenum));

CREATE TABLE patient (ohipnum CHAR(9) NOT NULL,firstname VARCHAR(20) NOT NULL, lastname VARCHAR(20) NOT NULL,birthdate DATE NOT NULL, PRIMARY KEY(ohipnum));

CREATE TABLE hospital (hoscode CHAR(3) NOT NULL, hosname VARCHAR(30) NOT NULL, city VARCHAR (20) NOT NULL, prov CHAR(2) NOT NULL, numofbed SMALLINT NOT NULL, headdoc CHAR(4),headdocstartdate DATE,  PRIMARY KEY (hoscode),   FOREIGN KEY (headdoc) REFERENCES doctor(licensenum));

-- add the foreign key
ALTER TABLE doctor ADD CONSTRAINT fk_worksat FOREIGN KEY (hosworksat) REFERENCES hospital(hoscode);

CREATE TABLE looksafter(licensenum CHAR(4) NOT NULL, ohipnum CHAR(9) NOT NULL, PRIMARY KEY (licensenum, ohipnum), FOREIGN KEY(licensenum) REFERENCES doctor(licensenum) ON DELETE RESTRICT, FOREIGN KEY (ohipnum) REFERENCES patient(ohipnum) ON DELETE CASCADE);

SHOW TABLES;

-- ------------------------------------
-- insert some data

-- insert into the Patients Table
SELECT * FROM patient;
INSERT INTO patient VALUES ('110112113','Monica','Geller','1964-05-12');
INSERT INTO patient VALUES ('444555666','Ross','Geller','1967-08-12');
INSERT INTO patient VALUES ('111222333','Rachel','Green','1962-09-17');
INSERT INTO patient VALUES ('333444555','Chandler','Geller','1970-06-11');
INSERT INTO patient VALUES ('667766777','Joey','Bing','1971-06-20');
INSERT INTO patient VALUES ('111222111','Phoebe','Bing','1959-12-24');
SELECT * FROM patient;

-- insert into the Hospital Table
SELECT * FROM hospital;
INSERT INTO hospital VALUES ('BBC','St. Joseph', 'London', 'ON', 1000, NULL, NULL);
INSERT INTO hospital VALUES ('ABC','Victoria', 'London', 'ON', 1600, NULL, NULL);
INSERT INTO hospital VALUES ('DDE','Victoria', 'Victoria', 'BC', 1200, NULL, NULL);
SELECT * FROM hospital;

-- insert into the Doctor Table
SELECT * FROM doctor;
INSERT INTO doctor VALUES ('RD34','Bernie', 'Kavorikian','1980-09-09', '1930-06-11','BBC','Urologist');
INSERT INTO doctor VALUES ('GD56','Joey', 'Shabado','1960-06-24', '1969-06-24','BBC','Podiatrist');
INSERT INTO doctor VALUES ('HT45','Ross', 'Clooney','1987-06-20', '1940-06-22','DDE','Surgeon');
INSERT INTO doctor VALUES ('YT67','Ben', 'Spock','1955-02-20', '1930-06-11','DDE','Urologist');
INSERT INTO doctor VALUES ('JK78','Mandy', 'Webster','1990-09-08', '1969-10-11','BBC','Surgeon');
INSERT INTO doctor VALUES ('SE66','Colleen', 'Aziz','1989-08-24', '1999-01-26','ABC','Surgeon');
SELECT * FROM doctor;

-- insert into the looksafter Table
SELECT * FROM looksafter;
INSERT INTO looksafter VALUES ('GD56','110112113');
INSERT INTO looksafter VALUES ('GD56','333444555');
INSERT INTO looksafter VALUES ('GD56','667766777');
INSERT INTO looksafter VALUES ('HT45','444555666');
INSERT INTO looksafter VALUES ('JK78','667766777');
INSERT INTO looksafter VALUES ('JK78','111222333');
INSERT INTO looksafter VALUES ('SE66','111222333');
INSERT INTO looksafter VALUES ('YT67','111222333');
INSERT INTO looksafter VALUES ('YT67','111222111');
SELECT * FROM looksafter;
