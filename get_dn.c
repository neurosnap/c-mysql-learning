#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#define STRING_SIZE 256

int main(int argc, char *argv[])
{

	MYSQL *mysql;
	MYSQL_STMT *stmt;
	MYSQL_BIND param[1], result[2];
	
	char *server = getenv("MYSQL_SERVER");
	char *user = getenv("MYSQL_USER");
	char *pass = getenv("MYSQL_PASS");
	char *db = getenv("MYSQL_DB");
	char *query;

	// setup variables for preparing and binding sql results
	char str_data[STRING_SIZE];
	unsigned long str_length;
	unsigned long length[2];
	int param_count, row_count;
	int int_data;
	my_bool is_null[2];
	my_bool error[2];
	
	mysql = mysql_init(NULL);
	
	if (mysql_real_connect(mysql, server, user, pass, db, 0, NULL, 0) == NULL)
	{
		fprintf(stderr, "No connection could be made to the database\n");
		return 1;
	}

	query = "SELECT `recordID`, `fname`"
	      " FROM `death_notices`"
	      " WHERE `bdate` = ? AND (`subclass` = '0010' OR `subclass` = '6320LOE')"
	      " ORDER BY `recordID` DESC";

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
	fprintf(stdout, " total parameters in SELECT: %d\n", param_count);

	// check parameter count
	if (param_count != 1)
	{
		fprintf(stderr, " invalid parameter count returned by MySQL\n");
		return 1;
	}

	memset(param, 0, sizeof(param));
	memset(result, 0, sizeof(result));

	// `bdate` prepared parameter VARCHAR COLUMN
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = (char *)str_data;
	param[0].buffer_length = STRING_SIZE;
	param[0].is_null = 0;
	param[0].length = &str_length;

	// `recordID` INTEGER COLUMN
	result[0].buffer_type = MYSQL_TYPE_LONG;
	result[0].buffer = (char *)&int_data;
	result[0].is_null = &is_null[0];
	result[0].length = &length[0];
	result[0].error = &error[0];

	// `fname` STRING COLUMN
	result[1].buffer_type = MYSQL_TYPE_STRING;
	result[1].buffer = (char *)str_data;
	result[1].buffer_length = STRING_SIZE;
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

	strncpy(str_data, "2014-10-09", STRING_SIZE);
	str_length = strlen(str_data);

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
			fprintf(stdout, " NULL\n");
		} else {
			fprintf(stdout, " %d(%ld)\n", int_data, length[0]);
		}

		if (is_null[1])
		{
			fprintf(stdout, " NULL\n");
		} else {
			fprintf(stdout, " %s(%ld)\n", str_data, length[1]);	
		}

		fprintf(stdout, "\n");
	}

	fprintf(stdout, "Total records: %d\n", row_count);

	if (mysql_stmt_close(stmt))
	{
		fprintf(stderr, " failed while closing the statement\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		return 1;
	}

	return 0;
}
