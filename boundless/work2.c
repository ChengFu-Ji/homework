#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>

int main() {
        //定義、初始化、創立連線控制代碼
	MYSQL *conn_ptr;

	conn_ptr = mysql_init(NULL);

	if (!conn_ptr)
	{
		fprintf(stderr, "mysql_init falied\n");
		return EXIT_FAILURE;
	}
        //建立連接
	conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root", "wang0907092300", "my_db", 0, NULL, 0);

	if (conn_ptr)
	{
		printf("Connection success\n");
	}
	else
	{
		printf("Connection failed\n");
	}
        //關閉連接
	mysql_close(conn_ptr);

	return EXIT_SUCCESS;
}
