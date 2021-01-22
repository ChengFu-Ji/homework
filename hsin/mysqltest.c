/*****************************************************************************************************************
資料庫連線程式:

	初始化資料庫(mysql_init(NULL))，分配address給conn_ptr，
		若分配失敗，印出 "init failed" ，並返回1(非零)，結束程式。

	成功後使用 mysql_real_connect 填寫 sql 資訊，
		若函式結果不等於NULL，印出success並使用 mysql_close(conn_ptr) 關閉 mysql 連線，
			印出 "connection closed"，結束程式。

		但 mysql_real_connect 結果為 NULL 時，印出 "failed"，如果 mysql_errno(conn_ptr) 非零，
			印出 "connect erro : 錯誤代碼 錯誤訊息 "，結束程式。

註:執行時需要 加上 -lmysqlclient 以使用函式庫

******************************************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include "/usr/include/mysql/mysql.h"

int main()
{
	MYSQL *conn_ptr;
	conn_ptr = mysql_init(NULL);

	if(!conn_ptr)
	{
		printf("init failed\n");		
		return 1;
	}

	if(mysql_real_connect(conn_ptr,"localhost","root","raspberry","ex",0,NULL,0)!=NULL)
	{
		printf("success\n");
		mysql_close(conn_ptr);
		printf("connection closed\n");
	}
	else
	{
		printf("failed\n");
		if(mysql_errno(conn_ptr))
		{
			printf("connect erro:%d %s\n",mysql_errno(conn_ptr),mysql_error(conn_ptr));
		}
		return 2;
	}
	return 0;
}

