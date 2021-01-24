#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<mariadb/mysql.h>

int main(){  

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;


	char *server = "127.0.0.1";
	char *user = "pi";
	char *password = "1234";
	char *database = "testsql2";

	conn = mysql_init(NULL);


	//連接資料庫
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
		fprintf(stderr,"%s\n",mysql_error(conn));
		exit(1);
	}

	if(mysql_query(conn,"show tables")){
		fprintf(stderr,"%s\n",mysql_error(conn));
		exit(1);
	}
	res = mysql_use_result(conn);

	//輸出資料表
	printf("Tables:\n");
	while((row = mysql_fetch_row(res)) != NULL){
		printf("%s\n",row[0]);
	}

	//關掉連線
	mysql_free_result(res);
	mysql_close(conn);
	


}
