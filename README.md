# MyDatabase
A mysql-like database using c++

# What's this?
This is a database writen in c++, the skeleton is given by ucsd ece141b. 
The database realized the basic function of a database, which is similar to mysql.

# How to use it?
Download the folder and click run, before you do that:
1. make sure that you change the make file to your version
2. set the environment variable: 'DB_PATH' : the path you want to store your db file
3. feel free to change anything you want, the code may has some bugs and forget to care about some corner case.
4. some package import might only works in macos, c++ 17. if you use linux and windows, revise them. 
5. all the command syntax is similar to mysql, if you have syntax errors, let your code follow the rule of mysql commands.

# functions:
* help -- get command help
* version -- get the current version of the database
* quit -- quit the program

* create database <db-name> -- create a .db file into the path you set in 'DB_PATH'
* use database <db-name> -- open the file, and set it to your active adatabase
* drop database <db-name> -- drop this database
* describe database <db-name> -- show a database like this:

* show databases -- show all the database in your path

* create table <tab-name> (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(100), email VARCHAR(100)) (<col-name> int|double|string, ...) -- create a table by given attributes and their type

* drop table <tab-name> -- drop a table

* insert into users (name, email) values ('foo', 'foo@ucsd.edu'), ('bar', 'bar@ucsd.edu') -- insert data into a table









{
      "name": "Select Limit Row Count",
      "setup": "make",
      "run": "export DB_PATH=/tmp && ./autograder",
      "input": "create database test4\r\nuse database test4\r\nCREATE TABLE users (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(100), email VARCHAR(100))\r\ninsert into users (name, email) values ('foo', 'foo@ucsd.edu'), ('bar', 'bar@ucsd.edu'), ('foobar', 'foobar@ucsd.edu'), ('anotherFoo', 'anotherfoo@ucsd.edu'), ('anotherBar', 'anotherbar@ucsd.edu'), ('anotherFooBar', 'anotherFoobar@ucsd.edu')\r\nselect * from users LIMIT 4\r\nquit",
      "output": "4 rows in set",
      "comparison": "included",
      "timeout": 1,
      "points": 5
    },






 





