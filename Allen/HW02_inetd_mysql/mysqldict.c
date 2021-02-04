/*
*C使用API連線Mysql資料庫:
*   
*	1.MYSQL *mysql：初始化MYSQL
*	2.mysql_real_connect：嘗試連線Mysql資料庫
*   3.mysql_query：執行SQL語法查詢
*	4.mysql_store_result：獲得查詢的結果
*	5.mysql_fetch_row：讀取查詢的結果
*	6.mysql_free_result：釋放查詢結果變數
*
*   @param  #define     mysql_IP                Mysql IP位置
*   @param  #define     mysql_Account           Mysql 登入帳號
*   @param  #define     mysql_Password          Mysql 登入密碼
*   @param  #define     mysql_Database          Mysql 資料庫名稱
*   @param  #define     mysql_Port         	    Mysql Port號
*	@param  MYSQL       *mysql         	        Mysql 變數
*   @param  char        MYSQL_Inquire           Mysql 查詢字串
*   @param  MYSQL_RES   *MYSQL_Result            Mysql 查詢結果 
*   @param  MYSQL_ROW   Data_Row         	    Mysql 查詢結果內容
*   @param  char        input                   使用者輸入之國家編號
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql/mysql.h"
#define mysql_IP "localhost"
#define mysql_Account "root"
#define mysql_Password ""
#define mysql_Database "ConnectTest"
#define mysql_Port 0

int main()
{
    MYSQL *mysql;
    mysql = mysql_init(NULL);
    char MYSQL_Inquire[] = "select * from country where country_id =";
    MYSQL_RES *MYSQL_Result;
    MYSQL_ROW Data_Row;
    char input[] = "";

    printf("請輸入國家編號：");
    scanf("%s", &input);
    strcat(MYSQL_Inquire, input);

    if (!mysql)
    {
        printf("Mysql Initialization Failed\n");
        return 0;
    }

    if (mysql_real_connect(mysql, mysql_IP, mysql_Account, mysql_Password, mysql_Database, mysql_Port, NULL, 0) != NULL)
    {
        printf("Mysql Connection Success\n");
    }
    else
    {
        printf("Mysql Connection Failed\n");
        if (mysql_errno(mysql))
        {
            printf("Connect Error Message:%d %s\n", mysql_errno(mysql), mysql_error(mysql));
        }
        return 2;
    }

    if (mysql_query(mysql, MYSQL_Inquire) == 0)
    {
        if ((MYSQL_Result = mysql_store_result(mysql)) != NULL)
        {
            while ((Data_Row = mysql_fetch_row(MYSQL_Result)) != NULL)
            {
                printf("查詢結果：%s\t\n", Data_Row[1]);
            }
            if ((Data_Row = mysql_fetch_row(MYSQL_Result)) == NULL)
            {
                printf("查詢結果：查無國家\n");
            }
            mysql_free_result(MYSQL_Result);
        }
        else
        {
            printf("Mysql Failed To Store Result!\n");
            return 4;
        }
    }
    else
    {
        printf("Mysql Failed To Query!\n");
        return 3;
    }

    mysql_close(mysql);
    printf("Mysql Connection Closed\n");
    return 1;
}
