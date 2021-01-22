#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mysql/mysql.h>

int cmddet(char *sqlcmd);	/*確認指令是show 或者 select 用的 function*/
void seekres(MYSQL *conn);	/*搜尋結果並顯示於CLI的 function*/

int main(int argc, char *argv[]) {
	char *sql;			
	char cmd[100];		/*指令的儲存空間*/	
	int res;			/*指令輸出結果*/		
	MYSQL *conn, init;		

	sql = cmd;		
	mysql_init(&init);		
	conn = mysql_real_connect(&init, "localhost", "pi", "qaz7613163", "project", 0, NULL, 0); 

	if(conn!=NULL) {									
		printf("connection success!\n");
	}else{
		printf("connection failed...\n");
		return 1;
	}

	while(1) {									
		fgets(sql, 100, stdin); 			

		if(strncmp(sql, "exit", 4)==0) {		/*離開輸入指令*/	
			printf("bye~\n");
			break;
		}

		res = mysql_query(conn, sql);			/*接收sql指令回傳值*/
	
		if(res) {								
			perror("my_query");				
			continue;						
		}else {
			if(cmddet(sql)) {			
				seekres(conn);
			}else {							
				printf("done!\n");
			}
		}
	}
	mysql_close(conn);						
	return 0;
}
void seekres(MYSQL *conn) {
	MYSQL_RES *res_ptr;					
	MYSQL_FIELD *field;					
	MYSQL_ROW result_row;			
	int row, column;			
	int i, j;						

	res_ptr = mysql_store_result(conn);		

	if(res_ptr!=NULL) {							
		column = mysql_num_fields(res_ptr);	
		row = mysql_num_rows(res_ptr);		
		printf("%d rows in set.\n", row);	

		for(i=0; (field = mysql_fetch_field(res_ptr)); i++) { 	/*列印搜尋結果欄位名*/
			printf("%10s", field->name);	
		}
		puts("\n");

		for(i=1; i<=row; i++) {									/*列印搜尋結果資料*/	
			result_row = mysql_fetch_row(res_ptr); 		
			for(j=0; j<column; j++) {					
				printf("%10s", result_row[j]);
			}
			puts("\n");
		}
	}
	mysql_free_result(res_ptr);
}

int cmddet(char *sqlcmd) {						
	char select[] = "select";
	char SELECT[] = "SELECT";
	char show[] = "show";
	char SHOW[] = "SHOW";

	if(!(strncmp(select, sqlcmd, 6)) || !(strncmp(show, sqlcmd, 4)) || 
	!(strncmp(SELECT, sqlcmd, 6)) || !(strncmp(SHOW, sqlcmd, 4))) {  
		return 1;
	} 
	return 0;
}

