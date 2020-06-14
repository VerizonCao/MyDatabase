# ECE141b -- Spring 2020 Final
## Altering a table
<hr>

We've all worked hard this term, to build a working database. Your last task, and an essential ingredient of life, is to deal with change. For this task, you will implement the `alter table |name| add |field_name| |type|` command, which alters an entity, and updates the associated row column(s) in your table.  After updating your code, you will execute the following statements:


```
CREATE DATABASE final
CREATE TABLE Names (id integer AUTO_INCREMENT PRIMARY KEY, first_name VARCHAR(30) NOT NULL)
INSERT INTO Names (first_name) VALUES ("savya"), ("chandhini"), ("rickg")
SELECT * from Names order by first_name
ALTER TABLE Names add last_name varchar(25)
UPDATE Names set last_name="Gessner" where id=3
UPDATE Names set last_name="Grandhi" where id=2
SELECT * from Names order by last_name
```

Your output will look (approximately) like this:
```
select * from Names order by first_name
+-----------+----------------------+
| id        | first_name           |
+-----------+----------------------+
| 1         | savya                |
+-----------+----------------------+
| 2         | chandhini            |
+-----------+----------------------+
| 3         | rickg                |
+-----------+----------------------+

//any output from altering table here..
//any output from updating your table here...

select * from Names order by first_name
+-----------+----------------------+-------------------+
| id        | first_name           | last_name         |
+-----------+----------------------+-------------------+
| 2         | chandhini            | grandhi           |
+-----------+----------------------+-------------------+
| 3         | rickg                | gessner           |
+-----------+----------------------+-------------------+
| 1         | savya                |                   |
+-----------+----------------------+-------------------+
```


## Step 1 - Setting up your project

The good news, is that the `Tokenizer`, and `AlterStatement` classes have all been updated for you in advance. Yay!  

Copy all your files from Assignment 9 into this your final project folder. Be sure not to overwrite the following files:

```
makefile 
student.json
test.sql
```

## Step 2 - 

### 2A: Create a new "AlterStatement" class to your system

In order to support the `alter table...` you'll need to create a new class to handle that statement type. Choose whatever name you like. 

### 2B: Add code to implement the "Alter table" command

The new feature requires the addition of one new method to one of your controller classes, typically `Database.hpp`. Call this method when it's time to execute an `AlterTable...` statement.


## Step 3 - Developing your solution

The key to understanding this challenge, is to recognize that the `alter table` command actually affects two things. First, it changes the definition of a known `Schema`, adding a new attribute.  Second, it changes _every_ existing row in your database associated with this `Schema`, and adds a new (empty) value for the newly added attribute (`last_name` in this case). 

It's very likely you already most of the code you need, from the commands you've already implemented. 

> DON'T PANIC -- if this is overwhelming, just do your best. A working solution is a great result. But if you can't accomplish that, then you can still get partial credit by writing methods with lots of comments that indicate what you would do. 

> ALTER TABLE is capable of making three types of changes to an entity: 1) ADD column; 2) DROP column; 3) MODIFY existing column. For your final, you are ONLY asked to alter your table using ADD column.

### Changing your `Schema`

To change your schema, load it from storage (or use the one you have in memory if it's already loaded).  Add the new attribute, and resave the entity to storage, using its original block_number. 

### Changing your rows

You could conventiently get a list of `Rows` by invoking `select * from table`. Now you have a RowCollection (or similar) list of `Rows`. For each element in your collection, add a new column (`last_name`) with a default value of "". Then save the row out, as if you were doing and `update` to its original block_number.  


## Part 4 - Submitting your solution

As usual, update the settings in your student.json file. Then commit your changes to github. 

Good luck everyone.



