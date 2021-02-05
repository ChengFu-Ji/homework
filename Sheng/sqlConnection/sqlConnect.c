#include <stdio.h>
#include <mysql/mysql.h>
/*
 *	 void seekres function接收資料並顯示結果
 *	 int cmddet function判斷是否需要顯示結果 
 *	 int strncmp function實作能包容大小寫不相同的 strcmp
 */
void seekres (MYSQL *conn);			
int cmddet (char *str);		
int strncmp (char *s1, char *s2, int n);	
/*	
 *	char cmdspace 儲存指令的空間
 *	int rsp 用於接收指令執行是否成功
 *	MYSQL *conn 用於連接及初始化
 */	
int main (int argc, char *argv[]) {
    char *sqlcmd;		
    char cmdspace[50];		
    char host[] = "localhost";
    char user[] = "account";
    char psw[] = "password";
    int rsp;
    MYSQL *conn;

    sqlcmd = cmdspace;

    conn = mysql_init(NULL);
    if (conn != NULL) {
        printf("initialize success!\n");
    } else {
        fprintf(stderr,"initialize failed...\n");
        return 1;
    }

    conn = mysql_real_connect(conn, host, user, psw, NULL, 0, NULL, 0);
    if (conn != NULL) {
        printf("connection success!\n");
    } else {
        fprintf(stderr, "connection failed...\n");
        return 1;
    }
    puts("Welcome!");

    while (1) {
        fgets(sqlcmd, 50, stdin);

        if (!strncmp(sqlcmd, "exit", 4)) {
            printf("Bye~\n");
            break;
        }

        rsp = mysql_query(conn, sqlcmd);
        if(rsp) {
            fprintf(stderr, "ERROR %d : %s\n", mysql_errno(conn), mysql_error(conn));
        }else {
            if (cmddet(sqlcmd)) {
                seekres(conn);
            } else {
                printf("done! Affected %d rows\n", (int) mysql_affected_rows(conn));
            }
        }
    }
    mysql_close(conn);
    return 0;
}

/*
 *	MYSQL_RES *res 儲存需要顯示的sq指令資訊
 *	MYSQL_FIELD *field_now 儲存該欄位資料
 *	MYSQL_ROW row_now 儲存該行資料
 */
void seekres (MYSQL *conn) {
    MYSQL_RES *res;
    MYSQL_FIELD *field_now;
    MYSQL_ROW row_now;
    int row, col;
    int i, j;

    res = mysql_store_result(conn);
    if (res != NULL) {
        row = mysql_num_rows(res);
        col = mysql_num_fields(res);

        for (i=0; (field_now = mysql_fetch_field(res)); i++) {
            printf("%15s", field_now->name);
        }
        puts("\n");
        for (i = 1; i <= row; i++) {
            row_now = mysql_fetch_row(res);
            for (j = 0; j < col; j++) {
                printf("%15s", row_now[j]);
            }
            puts("\n");
        }
    }
    mysql_free_result(res);
}

int cmddet (char *str) {
    char select[] = "select";
    char show[] = "show";
    char describe[] = "describe";

    if (!strncmp(select, str, 6) || !strncmp(show, str, 4) || !strncmp(describe, str, 8)) {
        return 1;
    }
    return 0;
}

int strncmp (char *s1, char *s2, int n) {
    int cmp = 0;

    while (n--) {
        cmp = *(s1++) - *(s2++);
        if ((cmp != 32) && (cmp != -32) && (cmp != 0)) {
            return cmp;
        }
    }
    return cmp;
}
