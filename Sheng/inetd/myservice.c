#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
/*
 *	seekres 用於接收sql指令回傳值並顯示的 function
 *	wordck 用於檢察輸入的每個字是否為英文字的 function
 */
void seekres(MYSQL *conn);
int wordck(char *word);

/*	*sqlcmd 用於傳送完整指令 
 *	srchsp 儲存要查單字的空間
 *	select 用於搜尋單字的指令
 *	*conn 用於連接sql
 */
int main (int argc, char argv[]) {
	char *srchptr, *sqlcmd;
	char srchsp[50];
	/*char buffer[BUFSIZ];*/
	char host[] = "localhost";
	char user[] = "pi";
	char psw[] = "qaz7613163";
	char database[] = "project";
	int rsp, len;
	MYSQL *conn;
	
	srchptr = srchsp;
	
	/*setvbuf(stdout, buffer, _IOLBF, BUFSIZ);*/

	fprintf(stdout,"\nWelcome to Sheng's dirctionary!!!\n");


	conn = mysql_init(NULL);
	if (conn != NULL) {
		printf("\nmysqlConnect initialize success!\n");
	}else {
		fprintf(stderr, "\nmysqlConnect initialize failed...\n");
		return 1;
	}

	conn = mysql_real_connect(conn, host, user, psw, database, 0, NULL, 0);
	if (conn != NULL) {
		printf("mysqlConnect connect success!\n");
	}else {
		fprintf(stderr, "mysqlConnect connect failed...\n");
		return 1;
	}

	rsp = mysql_query(conn, "set names utf8");
	if (rsp) {
		printf("\nerr: %s\n",mysql_error(conn));
	}else {
		printf("\nset utf8 success\n");
	}
		
	printf("\ntype in '--exit' to exit the service\n");
	printf("start to search!\n");


	while (1) {
		printf(">> ");

		fflush(stdout);
		scanf("%s", srchptr);

		if(!strncmp(srchptr, "--exit", 6)) {
			printf("exit the service. BYE~\n");
			break;
		}
		
		if(!wordck(srchptr)) {
			printf("Wrong input! Try again!\n");
			continue;
		}
		sqlcmd = (char *)malloc(50);

		sprintf(sqlcmd, "select * from vocab where en = '%s'", srchptr);

		rsp = mysql_query(conn, sqlcmd);
		if (rsp) {
			printf("err: %s\n",mysql_error(conn));
		}else {
			seekres(conn);
		}
		free(sqlcmd);
	}
	mysql_close(conn);
	return 0;
}
/*	*res 儲存執行sql指令回傳結果
 *	row_seek 儲存該行資訊(中文 及 英文)
 *	表格欄位設定 id, en(英文), ch(中文)
 */
void seekres (MYSQL *conn) {
	MYSQL_RES *res;
	MYSQL_ROW row_seek;
	int i;

	res = mysql_store_result(conn);
	row_seek = mysql_fetch_row(res);
	

	if (mysql_num_rows(res) == 0) {
		printf("Dictionary does not included this word\n");
		mysql_free_result(res);
		return;
	}

	for (i = 1; i < mysql_num_fields(res); i++) {
		printf("%s ", row_seek[i]);
	}
	printf("\n");
	mysql_free_result(res);
}

int wordck (char *word) {
	int i = 0;
	for (; *(word+i) != '\0'; i++) {
		if (((int) *(word+i) <= 64) || ((int) *(word+i) >= 91 && (int) *(word+i) <= 96) || ((int) *(word+i) >= 123)) {
			return 0;
		}
	}
	return i;
}
