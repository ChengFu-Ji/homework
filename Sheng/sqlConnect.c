#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mysql/mysql.h>

int commandCk(char *sqlCommand);
void showData(MYSQL *conn);
void upper(char* string);

int main(int argc, char *argv[]) {
	char *sql;
	char command[100];
	int res;
	MYSQL *conn, init;

	sql = command;
	mysql_init(&init);
	conn = mysql_real_connect(&init, "localhost", "root", "password", "project", 0, NULL, 0);

	if(conn) {
		printf("connection success!\n");
	}else{
		printf("connection failed...\n");
		return 1;
	}

	while(1) {
		fgets(sql, 100, stdin);

		if(strncmp(sql, "exit", 4)==0) {
			printf("bye~\n");
			break;
		}

		res = mysql_query(conn, sql);	
	
		if(res) {
			perror("my_query");
			continue;
		}else {
			if(commandCk(sql)!=0) {
				showData(conn);
			}else {
				printf("done!\n");
			}
		}
	}
	mysql_close(conn);
	return 0;
}
void showData(MYSQL *conn) {
	MYSQL_RES *res_ptr;
	MYSQL_FIELD *field;
	MYSQL_ROW result_row;
	int row, column;
	int i, j;

	res_ptr = mysql_store_result(conn);

	if(res_ptr) {
		column = mysql_num_fields(res_ptr);
		row = mysql_num_rows(res_ptr);
		printf("Searched %d rows\n", row);

		for(i=0; (field = mysql_fetch_field(res_ptr)); i++) {
			printf("%10s", field->name);
		}
		puts("\n");

		for(i=1; i<=row; i++) {
			result_row = mysql_fetch_row(res_ptr);
			for(j=0; j<column; j++) {
				printf("%10s", result_row[j]);
			}
			puts("\n");
		}
	}
}

int commandCk(char *sqlCommand) {
	char select[] = "select";
	char SELECT[] = "SELECT";
	char show[] = "show";
	char SHOW[] = "SHOW";

	if(strncmp(select, sqlCommand, 6)==0 || strncmp(show, sqlCommand, 4)==0 ||
	strncmp(SELECT, sqlCommand, 6)==0 || strncmp(SHOW, sqlCommand, 4)==0) { 
		return 1;
	} 
	return 0;
}

