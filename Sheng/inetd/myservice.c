#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int strlens(char *s);
void strcp(char *s1, char *s2);
void seekres(MYSQL *conn);
void straddc (char *s1, char c);
int strncmp (char *s1, char *s2, int n);

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

		len = strlens(select) + strlens(srchptr) + 2;
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

int strlens (char *s) {
	int i = 0;
	while (1) {
		if (*(s+i) != '\0') {
			i++;
		}else {
			break;
		}
	}
	return i;
}

void strcp (char *s1, char *s2) {
	int len = strlens(s1);
	int i;
	for (i = 0; *(s2+i) != '\0'; i++) {
		*(s1+(len++)) = *(s2+i); 
	}
	*(s1+len) = '\0'; 
}

void straddc (char *s1, char c) {
	int len = strlens(s1);
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
