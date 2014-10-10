#include <stdio.h>
#include <string.h>
#include <mysql.h>

int main(int argc, char *argv[])
{
	printf("MySQL client version: %s\n", mysql_get_client_info());

	MYSQL *con = mysql_init(NULL);
	char db[] = "testdb";
	char query[] = "CREATE DATABASE IF NOT EXISTS ";
	strncat(query, db, sizeof(db));
	printf("%s\n", query);

	if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		return 1;
	}

	if (mysql_real_connect(con, "localhost", "erock", "pass123",
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
