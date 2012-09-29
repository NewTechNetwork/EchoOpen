Installing Echo Open

The following instructions are intended to provide a step by step guide for installing the Echo Open web application. Before beginning multiple things are assumed:
•	A new copy of Drupal 6.14 has been downloaded and placed in your root directory
•	Ability to create a database

Modifying Drupal’s core

There are several files within the Drupal core that require modifications, these files have been modified for you, located in the modifications folder provided, and must be placed in the original installation directory. For instance, you have been provided with an includes folder that contains two files bootstrap.inc and commons.inc. These files are to be copied to their corresponding folder within your base installation overwriting existing files. This process should be repeated for the other folders containing modified files.

Installing the database

Provided within the Echo Open package is an SQL dump file, this file must be imported to your database environment. Handling SQL dump files is outside of the scope of this guide but it is recommended if you are having trouble you consider downloading a DBMS such as MySQL Workbench to assist you with this task.

Editing settings.php

The final step when installing the Echo Open package is to edit the settings.php file which contains the database connection string, this file is located at /sites/default/settings.php relative to the root directory of your site. You will need to edit the database connection based on your specific database credentials and database name. Once completed save this file.

Application Setup

Once the package is installed, login with the default credentials: username=admin and password=echoopen. Click on “Administer” to create new schools and manage the entire backend application (including the Google Apps integration). Once at least one school is setup, user accounts can be assigned to schools and then courses, groups, and other content can be added within a school. 

Congratulations! If you have followed these instructions successfully you will have a fully working installation of Echo Open.
