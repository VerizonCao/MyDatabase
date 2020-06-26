# MyDatabase
A mysql-like database using c++, writen by Zongheng Cao.

# What's this?
This is a database writen in c++, the skeleton is given by ucsd ece141b. 
The project realize some basic functions of a simple database: create, update, select, join......  which is similar to mysql.
The database is speed up by cache and index.

# How to use it?
Download the folder and click run, before you do that:
1. make sure that you change the make file to your version
2. set the environment variable: 'DB_PATH' : the path you want to store your db file
3. feel free to change anything you want, the code may has some bugs and forget to care about some corner case.
4. some package import might only works in macos, c++ 17. if you use linux and windows, revise them. 
5. all the command syntax is similar to mysql, if you have syntax errors, let your code follow the rule of mysql commands.

# When you type a command:
Firstly, the main fucntion handle the input and get tokens by given command.
Secondly, the Command processors decide which one is in charge of this command.
Thirdly, that certain processor create a Statement by that command (factory pattern). 
Fourthly, that statement parse tokens and use run to make some operations (show something or change something in database)

# functions:
### 1. App Commands:
* help -- get command help
* version -- get the current version of the database
* quit -- quit the program


### 2. Database Commands: 

* create database \<db-name\> -- create a .db file into the path you set in 'DB_PATH'
* use database \<db-name\> -- open the file, and set it to your active adatabase
* drop database \<db-name\> -- drop this database
* describe database \<db-name\> -- show a database like this:

```
describe databases class;
Blk#  Type    Other
----------------------------
0     Meta 
1     Schema  "users"
2     Data    id 1
3     Data    id 2 
```
* show databases -- show all the databases in path.

### 3. Table Commands: 


* create table \<tab-name\> (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(100), email VARCHAR(100)) (<col-name> int|double|string, ...) -- create a table by given attributes and their type

* drop table \<tab-name\> -- drop a table

* insert into users (name, email) values ('foo', 'foo@ucsd.edu'), ('bar', 'bar@ucsd.edu') -- insert data into a table

* select * from \<tab-name\>

```
> select * from users;
+---------+--------------------+--------------------+
| id      | firstname          | lastname           | 
+---------+--------------------+--------------------+
| 2       | A                  | B                  | 
+---------+--------------------+--------------------+
1 rows in set (0.000108458 ms.)
```
* show tables -- show all the tables in this database
```
show tables;
+----------------------+
| Tables_in_mydb       |
+----------------------+
| groups               |
| users                |
+----------------------+
2 rows in set 
```

* describe table \<tab-name\> -- describe the info of attributes
```
> DESCRIBE tasks;
+-----------+--------------+------+-----+---------+-----------------------------+
| Field     | Type         | Null | Key | Default | Extra                       |
+-----------+--------------+------+-----+---------+-----------------------------+
| id        | integer      | NO   | YES | NULL    | auto_increment primary key  |
| title     | varchar(100) | NO   |     | NULL    |                             |
| price     | float        | YES  |     | 0.0     |                             |
| due_date  | date         | YES  |     | NULL    |                             |
| status    | boolean      | YES  |     | FALSE   |                             |
+-----------+--------------+------+-----+---------+-----------------------------+
5 rows in set 
```

* delete from \<tab-name\> -- delete all the records associated with a table 

* select * from user order by name limit 5 -- select the user, sort by name and return 5 rows

* update users SET state = CA WHERE zip_code = 92127 -- update the value of some rows

* show indexes -- show all indexes in this db
```
show indexes;
+-----------------+-----------------+
| table           | field           | 
+-----------------+-----------------+
| users           | id              |  
+-----------------+-----------------+
1 rows in set 
```

* join: select users.first_name, users.last_name, order_number from users LEFT JOIN orders ON users.id=orders.user_id -- use join to cooperate two tables
```
> select last_name, title from Authors left join(right join) Books on Authors.id=Books.author_id;
+-----------+-------------------------------------------+
| last_name | title                                     |
+-----------+-------------------------------------------+
| Rowling   | Harry Potter and the Sorcerer's Stone     |
| King      | Carrie                                    |
| King      | The Dark Tower                            |
| King      | The Green Mile                            |
| Nguyen    | NULL                                      |
+-----------+-------------------------------------------+
5 rows in set (0.00 sec)
```

# Others:  

1. The select statement will be accelerated by index, a table at least has a primary key index. And may have some index to speed up like: name > 15. The in-memory index is a b+ tree structure, which is easy to find elements greater than 15
2. When the user use the same command, the processor will use the command cache to get the row Collections quickly if hit.
3. I use a in-memory LRU cache to store some rows. If a Row isn't used recently, it will be deleted from the cache.
4. contact with me at zocao@eng.ucsd.edu




