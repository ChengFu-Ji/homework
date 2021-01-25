#include <stdio.h>
#include <mysql.h>

#define SERVER "localhost"
#define USER "chengfu"
#define PASSWD "chengfu"
#define DATABASE "raw"

int main()
{
    MYSQL *connect;
    MYSQL_RES *res;
    MYSQL_ROW row;

#ifdef DEBUG
	fprintf(stdout, "\nStart MySQL INIT\n");	
#endif

    connect = mysql_init(NULL);

#ifdef DEBUG
	fprintf(stdout, "\nStart MySQL connection\n");	
#endif

    if (!mysql_real_connect(connect, SERVER, USER, PASSWD, DATABASE, 0, NULL, 0)) {
        fprintf(stderr, "(%d)[%s]\n", __LINE__, mysql_error(connect));
        return -1;
    }

#ifdef DEBUG
	fprintf(stdout, "\nStart MySQL query\n");	
#endif

    if (mysql_query(connect, "SHOW TABLES")) {
        fprintf(stderr, "(%d)[%s]\n", __LINE__, mysql_error(connect));
        return -1;
    }

    res = mysql_use_result(connect);

#ifdef DEBUG
	fprintf(stdout, "\nStart MySQL parse fetch row\n");	
#endif

    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%s, %s\n", row[0], row[1]);
    }

    mysql_free_result(res);
    mysql_close(connect);

    return 0;
}
