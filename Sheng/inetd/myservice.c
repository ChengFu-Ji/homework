#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
/*
 *	mystrlen 用於計算字串長度的 function
 *	strcp 用於複製字串的 function
 *	seekres 用於接收sql指令回傳值並顯示的 function
 *	straddc 用於增加字元在字串尾部 function
 *	strncmp 實做能兼容大小差異的strncmp
 */
int mystrlen(char *s);
void strcp(char *s1, char *s2);
void seekres(MYSQL *conn);
void straddc (char *s1, char c);
int strncmp (char *s1, char *s2, int n);

/*	*sqlcmd 用於傳送完整指令 
 *	srchsp 儲存要查單字的空間
 *	select 用於搜尋單字的指令
 *	comma 字元 `'` 用於搜尋指令字尾
 *	*conn 用於連接sql
 *
 *	setvbuf() 為了將標準輸出(stdout)的輸出模式改回line buffered (IOLBF) (猜測
 *	因為compiler後 直接執行該檔案 在預設的情況下標準輸出(stdout)的BUFFER模式通常為line buffered
 *	而掛上inetd後 經由telnet 連線 標準輸出(stdout)的BUFFER模式會變為fully buffered (猜測
 *	因為在本人將標準輸出(stdout)的BUFFER模式改為fully buffered 並compiler後 
 *	執行結果和在使用telnet連線後 輸出的結果一致 所以猜測 
 *	在經由telnet 連線後會將標準輸出(stdout)的BUFFER模式改為fully buffered
 */
int main (int argc, char argv[]) {
	char *srchptr, *sqlcmd;
	char srchsp[15];
	char buffer[BUFSIZ];
	char host[] = "localhost";
	char user[] = "pi";
	char psw[] = "qaz7613163";
	char database[] = "project";
	char select[] = "SELECT * FROM vocab WHERE en = '";
	char comma = 39;
	int rsp, len;
	MYSQL *conn;
	
	srchptr = srchsp;
	
	setvbuf(stdout, buffer, _IOLBF, BUFSIZ);

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
		scanf("%s", srchptr);

		if(!strncmp(srchptr, "--exit", 6)) {
			printf("exit the service. BYE~\n");
			break;
		}

		len = mystrlen(select) + mystrlen(srchptr) + 2;
		sqlcmd = (char *)malloc(len);
		*sqlcmd = '\0';

		strcp(sqlcmd, select);
		strcp(sqlcmd, srchptr);
		straddc(sqlcmd, comma);
		
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

int mystrlen (char *s) {
	int i = 0;
	while (1) {
		if (*(s+i) == '\0') {
			break;
		}else {
			i++;
		}
	}
	return i;
}

void strcp (char *s1, char *s2) {
	int len = mystrlen(s1);
	int i;
	for (i = 0; *(s2+i) != '\0'; i++) {
		*(s1+(len++)) = *(s2+i); 
	}
	*(s1+len) = '\0'; 
}

void straddc (char *s1, char c) {
	int len = mystrlen(s1);
	*(s1+(len++)) = c;
	*(s1+(len)) = '\0';
}

int strncmp (char *s1, char *s2, int n) {
	int cmp = 0, i=0;

	while (n--) {
		cmp = *(s1+i) - *(s2+i);
		i++;
		if (cmp!=32 && cmp!=-32 && cmp!=0) {
			return cmp;
		}
	}
	return cmp;
}
