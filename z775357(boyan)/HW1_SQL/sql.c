#include<mysql/mysql.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

void sentsql(char *sql,MYSQL *con)
	{
		int num_fields,i;
		MYSQL_RES *res;
		MYSQL_ROW row;
		MYSQL_FIELD *field;

		if (mysql_query(con,sql))
		{
			printf("%s\n", mysql_error(con));
			mysql_close(con);
			exit(1);
		}
		else
		{
			printf("sql sent success!!\n");
		}
		
		res=mysql_store_result(con);
		if (res != NULL){
		printf("---------------------------\n");
		num_fields= mysql_num_fields(res);
		field=mysql_fetch_fields(res);
		while (row = mysql_fetch_row(res))
		{
			for(i=0;i<num_fields;++i)
			{
				printf("[%s]=>[%s]\n",field[i].name,row[i]);
			}  
		}
		mysql_free_result(res);
		printf("---------------------------\n");
		}
}

int main(){
	MYSQL *con = mysql_init(NULL);
	char *sql = malloc(50);


	if (con == NULL)
	{
		printf("%s\n", mysql_error(con));
		printf("sql init failed!");
		exit(1);
	}
	else
		printf("sql init success\n");

	if (mysql_real_connect(con, "localhost", "root", "root","db1", 0, NULL, 0) == NULL)
	{
		printf("%s\n", mysql_error(con));
		printf("sql connect error!\n");
		mysql_close(con);
		exit(1);
	}  
	else
		printf("sql connect success!\n");

	while(1)
	{
		fgets(sql, 50, stdin);
		sentsql(sql,con);
	}
	free(sql);
	mysql_close(con);
	return 0;
}
