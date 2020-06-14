CREATE DATABASE final
use database final
CREATE TABLE Names (id integer AUTO_INCREMENT PRIMARY KEY, first_name VARCHAR(30) NOT NULL)
INSERT INTO Names (first_name) VALUES ("savya"), ("chandhini"), ("rickg")
SELECT * from Names order by first_name
ALTER TABLE Names add last_name varchar(25)
UPDATE Names set last_name="Gessner" where id=3
UPDATE Names set last_name="Grandhi" where id=2
SELECT * from Names order by last_name
quit
