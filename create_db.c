#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

int main(int argc, char *argv[])
{
	printf("MySQL client version: %s\n", mysql_get_client_info());

	MYSQL *con = mysql_init(NULL);	

	char *server = getenv("MYSQL_SERVER");
	char *user = getenv("MYSQL_USER");
	char *pass = getenv("MYSQL_PASS");
	char *db = getenv("MYSQL_DB");	

	char query[50] = "CREATE DATABASE IF NOT EXISTS ";
	strncat(query, db, strlen(db));
	printf("QUERY: %s\n", query);	
	
	if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		return 1;
	}

	if (mysql_real_connect(con, server, user, pass,
		NULL, 0, NULL, 0) == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		return 1;
	}

	if (mysql_query(con, query))
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		return 1;
	}

	puts("MySQL database created!");
	
	mysql_close(con);
	return 0;
}
