#include<stdio.h>
#include<stdlib.h>
#include"mysql/mysql.h"
#include <string.h>


int main()
{

    int t, r = 0,encode,count=0;
    char query[100] = "SELECT ch FROM translation WHERE en='"; 
    char quotation[] = "'";
    char *words;
    MYSQL *conn_ptr,init; //用於連線而宣告的變數
    MYSQL_RES *res;
    MYSQL_ROW row;

    //init a  SQL struct 
    conn_ptr = mysql_init(&init);
    
    if(!conn_ptr){
    
        fprintf(stderr,"mysql_init failed\n"); 
        return 1; 
    
    }

    //connect to SQL
    conn_ptr = mysql_real_connect(&init,"localhost","root","","test",0,NULL,0);

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
         
        words = (char *)malloc(50); 
       
        printf("input:\n"); 
        
        fflush(stdout);
        scanf(" %s",words);

        strcat(query,words);
     
        //離開程式
        if(strcmp(words,"exit") == 0)
        {
            break; 
        }
        
        if(count == 0)
        {
            strcat(query,quotation);
        }
        //更改sql的編碼，不然會造成接收的東西為亂碼
        encode = mysql_query(conn_ptr,"SET NAMES UTF8");
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
        //mysql_field_count 會將query 執行後有幾列回傳
        for(r = 0;r<= mysql_field_count(conn_ptr);r++)
        {
            // 回傳query 執行後一行的結果 , 此row 透過迴圈得到結果的下一行 
            row = mysql_fetch_row(res); 
    
            if(row == 0) 
            {
                break; 
            }    
            
            //mysql_num_fields(res) 會回傳該行有幾列
            for(t = 0; t < mysql_num_fields(res);t++)
            {
                printf(" %10s ",row[t]); 
            }
            printf("\n");
        }

        //釋放res中所查詢過的sql資料
        mysql_free_result(res); 
        free(words); 
        count++; 
        for(int i = 36; i <= strlen(query);i++) 
        {
              *(query + i) = (char)NULL; 
        } 
    }
    //斷開與SQL的連線 
    mysql_close(conn_ptr); 
    
    return 0;
}

