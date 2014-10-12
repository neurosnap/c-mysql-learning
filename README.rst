C MySQL API Learning
====================

Learning C through example

Pre-Reqs
========

* UNIX
* gcc (https://gcc.gnu.org/)
* libmysqlclient (https://dev.mysql.com/doc/refman/5.0/en/c-api.html)
* make (http://linux.die.net/man/1/make)

How-To
======

Set environment variables

.. code:: bash

    export MYSQL_SERVER="localhost"
    export MYSQL_USER="test_user"
    export MYSQL_PASS="testpass123"
    export MYSQL_DB="testdb"

Create database

.. code:: bash

    $ make create_db
    $ ./create_db

Import schema table and sample data

.. code:: bash

    $ mysqldump -u <user> -p <db> < sample_data/dn.sql

Get results

.. code:: bash

    $ make get_dn
    $ ./get_dn
