#include<stdio.h>
#include<stdlib.h>
#include<mysql/mysql.h>
/*
    mysql C API:
        mysql_init ���t�O���鵹��Ʈw
        �T�{���t�O����O�_���\��
        mysql_real_connect �s���ܸ�Ʈw
        �T�{��Ʈw�O�_�s�����\
        mysql_query ����sql�y�k
        mysql_use_result �o��d�ߵ��G
        �p�G�٦��Ѿl�����ƫh�d�ߵ��G
        �̫�N�O��������A�����{��
*/

int main()
{

    int i, j;
    MYSQL *mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    mysql = mysql_init(NULL);


    if(mysql == NULL)
    {
        printf("init ... failed\n");
        return 0;
    }
    mysql = mysql_real_connect(mysql,"localhost","root","","exercise_mysql",0,NULL,0);
    if(mysql)
    {
        printf("connect ... ok\n");
    }
    else
    {
        printf("connect ... failed\n");
    }

    if(mysql_query(mysql,"select * from class"))
    {
        printf("Query error!");
    }

    res = mysql_use_result(mysql);
    while( row = mysql_fetch_row(res))
    {
      for(j = 0; j < mysql_num_fields(res);j++)
        {
            printf(" %5s ",row[j]);
        }
//      printf(" %5s ",row[0]);
//      printf(" %5s ",row[1]);
        printf("\n");
    }
    mysql_free_result(res);
    mysql_close(mysql);
    return 0;
}
