#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#define STRING_SIZE 256

int main(int argc, char *argv[])
{

	if (argc != 3)
	{
		printf("Must supply start and end date: %s '2014-10-03' '2014-10-10'\n", argv[0]);
		return 0;
	}

	MYSQL *mysql;
	MYSQL_STMT *stmt;
	MYSQL_BIND param[2], result[2];

	char *server = getenv("MYSQL_SERVER");
	char *user = getenv("MYSQL_USER");
	char *pass = getenv("MYSQL_PASS");
	char *db = getenv("MYSQL_DB");
	char *query;

	// setup variables for preparing and binding sql results
	char str_data[STRING_SIZE][3];
	unsigned long str_length[2];
	int param_count, row_count;
	int int_data;
	unsigned long length[2];
	my_bool is_null[2];
	my_bool error[2];

	mysql = mysql_init(NULL);

	if (mysql_real_connect(mysql, server, user, pass, db, 0, NULL, 0) == NULL)
	{
		fprintf(stderr, "No connection could be made to the database\n");
		return 1;
	}

	query = "SELECT DAYNAME(`bdate`) as 'date', COUNT(*) as 'count'"
			" FROM `death_notices`"
			" WHERE `bdate` >= ? AND `bdate` <= ?"
			" 	AND (`subclass` = '0010' OR `subclass` = '6320LOE')"
			" GROUP BY DAYNAME(`bdate`)"
			" ORDER BY COUNT(*) DESC";

	// prepare sql query
	stmt = mysql_stmt_init(mysql);

	if (!stmt)
	{
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		return 1;
	}

	if (mysql_stmt_prepare(stmt, query, strlen(query)))
	{
		fprintf(stderr, " mysql_stmt_prepare(), SELECT failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return 1;
	}

	param_count = mysql_stmt_param_count(stmt);
	fprintf(stdout, "Total parameters in SELECT: %d\n", param_count);

	// check parameter count
	if (param_count != 2)
	{
		fprintf(stderr, " invalid parameter count returned by MySQL\n");
		return 1;
	}

	memset(param, 0, sizeof(param));
	memset(result, 0, sizeof(result));

	// `bdate` start date prepared parameter VARCHAR COLUMN
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = (char *)str_data[0];
	param[0].buffer_length = STRING_SIZE;
	param[0].is_null = 0;
	param[0].length = &str_length[0];

	// `bdate` end date prepared parameter VARCHAR COLUMN
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = (char *)str_data[1];
	param[1].buffer_length = STRING_SIZE;
	param[1].is_null = 0;
	param[1].length = &str_length[1];

	// `day name` STRING COLUMN
	result[0].buffer_type = MYSQL_TYPE_STRING;
	result[0].buffer = (char *)str_data[2];
	result[0].buffer_length = STRING_SIZE;
	result[0].is_null = &is_null[0];
	result[0].length = &length[0];
	result[0].error = &error[0];

	// `count` INTEGER COLUMN
	result[1].buffer_type = MYSQL_TYPE_LONG;
	result[1].buffer = (char *)&int_data;
	result[1].is_null = &is_null[1];
	result[1].length = &length[1];
	result[1].error = &error[1];

	if (mysql_stmt_bind_param(stmt, param))
	{
		fprintf(stderr, " mysql_stmt_bind_param() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return 1;
	}
	
	if (mysql_stmt_bind_result(stmt, result) != 0)
	{
		fprintf(stderr, " mysql_stmt_bind_result() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return 1;
	}

	strncpy(str_data[0], argv[1], STRING_SIZE);
	str_length[0] = strlen(str_data[0]);

	strncpy(str_data[1], argv[2], STRING_SIZE);
	str_length[1] = strlen(str_data[1]);

	if (mysql_stmt_execute(stmt))
	{
		fprintf(stderr, " mysql_stmt_execute(), failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return 1;
	}

	row_count = 0;
	fprintf(stdout, "Fetching results ...\n");	
	while (!mysql_stmt_fetch(stmt))
	{
		row_count++;

		if (is_null[0])
		{
			fprintf(stdout, "NULL\n");
		} else {
			fprintf(stdout, "%s: ", str_data[2]);
		}

		if (is_null[1])
		{
			fprintf(stdout, "NULL\n");
		} else {
			fprintf(stdout, " %d\n", int_data);	
		}

		fprintf(stdout, "\n");
	}

	if (mysql_stmt_close(stmt))
	{
		fprintf(stderr, " failed while closing the statement\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return 1;
	}

	return 0;

}