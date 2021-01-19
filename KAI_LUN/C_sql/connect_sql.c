#include<stdio.h>
#include<stdlib.h>
#include"mysql/mysql.h"
#include <string.h>

int main(int argc, const char *argv[])
{

    int t, r = 0;
    char *query;
    MYSQL *conn_ptr,init; //用於連線而宣告的變數
    MYSQL_RES *res;
    MYSQL_ROW row;

    query = (char *)malloc(50);
    //init a  SQL struct 
    conn_ptr = mysql_init(&init);
    
    if(!conn_ptr){
    
        fprintf(stderr,"mysql_init failed\n"); 
        return EXIT_FAILURE; 
    
    }

    //connect to SQL
    conn_ptr = mysql_real_connect(&init,"localhost","root","password","test",0,NULL,0);

    //判斷連線成不成功
    if(conn_ptr)
    {
        printf("Connection success\n"); 
    }
    else
    {
        printf("Connection failed\n"); 
    }
  
    while(1)
    {
        fgets(query,50,stdin); //鍵入字串給query
        
        //執行query 字串中的SQL 語句
        t = mysql_real_query(conn_ptr,query,(unsigned int)strlen(query));
    
        if(t)
        {
            printf("ERROR MAKING QUERY: %s\n",mysql_error(conn_ptr));
        }
        else
        {
            printf("query made...\n"); 
    
        }
    
        //將query 執行後的結果檢索給res 
        res = mysql_use_result(conn_ptr);
 
        //show the return is result
        for(r = 0;r<= mysql_field_count(conn_ptr);r++) //mysql_field_count 會將query 執行後有幾列回傳
        {
            row = mysql_fetch_row(res);// 回傳query 執行後一行的結果 , 此row 透過迴圈得到結果的下一行 
    
            if(row == 0) 
            {
                break; 
            }    
        
            for(t = 0; t < mysql_num_fields(res);t++)//mysql_num_fields(res) 會回傳該行有幾列
            {
                printf(" %10s ",row[t]); 
            }

            printf("\n");
        }

        mysql_free_result(res);//釋放res中所查詢過的sql資料 
        free(query);
    }
   
    mysql_close(conn_ptr); // 斷開與SQL的連線
    
    return 0;
}

