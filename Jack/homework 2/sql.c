#include<stdio.h>
#include<stdlib.h>
#include<mysql/mysql.h>
#include<string.h>
/*
    mysql C API:
	mysql_init 分配記憶體給資料庫
	確認分配記憶體是否成功後
	mysql_real_connect 連接至資料庫
	確認資料庫是否連結成功
	mysql_query 執行sql語法
	mysql_use_result 得到查詢結果
	如果還有剩餘的筆數則查詢結果
	最後將記憶體釋放，結束程式
*/

int main()
{

    int i, j;
    MYSQL *mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    mysql = mysql_init(NULL);
    char query[]="select result from query where keyword='";
    char input[]="";

    if(mysql == NULL)
    {
        printf("init ... failed\n");
        return 0;
    }
    mysql = mysql_real_connect(mysql,"localhost","root","root_password","en_letters",0,NULL,0);
    if(mysql)
    {
        printf("connect ... ok\n");
    }
    else
    {
        printf("connect ... failed\n");
    }
    scanf("%s",&input);
    strcat(query,input);
    strcat(query,"'");
    if(mysql_query(mysql,query))
    {
        printf("Query error!");
    }

    res = mysql_use_result(mysql);
    printf("-----------\n");
    while( row = mysql_fetch_row(res))
    {
      for(j = 0; j < mysql_num_fields(res);j++)
	    {
	       printf(" %5s ",row[j]);
	    }
	    printf("\n");
    }
    printf("----end----\n");
    mysql_free_result(res);
    mysql_close(mysql);
    return 0;
}
