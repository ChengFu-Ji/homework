#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysqllib.h"
#include "mysql/mysql.h"
#define mysql_IP "localhost"
#define mysql_Account "root"
#define mysql_Password ""
#define mysql_Database "ConnectTest"
#define mysql_Port 0

MYSQL *mysql;

int sqlcon(){
	mysql = mysql_init(NULL);
	if (!mysql)
    	{
        	printf("Mysql 初始化錯誤\n");
        	return 1;
    	}

    	if (mysql_real_connect(mysql, mysql_IP, mysql_Account, mysql_Password, mysql_Database, mysql_Port, NULL, 0) != NULL)
    	{
        	printf("Mysql 連線成功\n");
		return 0;
   	}
   	else
    	{
        	printf("Mysql 連線錯誤\n");
        	if (mysql_errno(mysql))
       		{
			printf("錯誤訊息:%d %s\n", mysql_errno(mysql), mysql_error(mysql));
        	}
        	return 1;
    	}
}

void countryselect(char id[]){
	MYSQL_RES *MYSQL_Result;
	MYSQL_ROW Data_Row;
	int dataflag = 0;
	char MYSQL_Inquire[] = "select * from country where country_id =";
	strcat(MYSQL_Inquire,id); 
	if (mysql_query(mysql, MYSQL_Inquire) == 0)
	{
		if ((MYSQL_Result = mysql_store_result(mysql)) != NULL)
		{
			while ((Data_Row = mysql_fetch_row(MYSQL_Result)) != NULL)
			{
				printf("查詢結果：%s\t\n", Data_Row[1]);
				dataflag = 1;
			}
			if(dataflag == 0)
			{
				printf("查詢結果：查無國家\n");
			}
			dataflag = 0;
			mysql_free_result(MYSQL_Result);
		}
		else
		{
			printf("Mysql 無法取得查詢結果\n");
		}
	}
	else
	{
		printf("Mysql 查詢失敗\n");
	}
}

void sqlclose(){
	mysql_close(mysql);
	printf("Mysql 連線關閉\n");
}
