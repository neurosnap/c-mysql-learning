#CFLAGS = -Wall -L/usr/lib/mysql/ -lmysqlclient -I/usr/include/mysql/

create_db:
        gcc create_db.c -o create_db `mysql_config --cflags --libs`

get_dn:
        gcc get_dn.c -o get_dn `mysql_config --cflags --libs`

analyze_dn:
        gcc analyze_dn.c -o analyze_dn `mysql_config --cflags --libs`


