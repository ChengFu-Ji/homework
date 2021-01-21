#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <mysql/mysql.h>

int commandCk(char *sqlCommand);
void showData(MYSQL *conn);

int main(int argc, char *argv[]) {
	char *sql;				//指向儲存指令空間的指標
	char command[100];		//儲存指令用空間
	int res;				//查看回傳結果是否正確的數值
	MYSQL *conn, init;		//conn 用於儲存連線資訊 init用於mysql_init及mysql_real_connect

	sql = command;			//將指標指向儲存用空間
	mysql_init(&init);		//初始化
	conn = mysql_real_connect(&init, "localhost", "root", "qaz7613163", "project", 0, NULL, 0); //建立連線

	if(conn) {									//判斷是否連線
		printf("connection success!\n");
	}else{
		printf("connection failed...\n");
		return 1;
	}

	while(1) {									//目的 為了能夠不斷的輸入指令直到exit
		fgets(sql, 100, stdin); 				//接收鍵盤輸入的字串

		if(strncmp(sql, "exit", 4)==0) {		//字串比對前4位只要出現exit(小寫)便退出程式
			printf("bye~\n");
			break;
		}

		res = mysql_query(conn, sql);		//對mariaDB進行指令並回傳指令是否成功的參數 0為成功 非0為失敗
	
		if(res) {							//if 判斷式中 0 為false 非0為 true 判斷是否錯誤	
			perror("my_query");				//列出錯誤 然後重新輸入	
			continue;						
		}else {
			if(commandCk(sql)) {			//如果檢測到使用者輸入是select或者show(大小寫皆可)則顯示回傳資料
				showData(conn);
			}else {							//否則表示完成
				printf("done!\n");
			}
		}
	}
	mysql_close(conn);						//當使用者輸入exit後 與 mariaDB 關閉鏈接
	return 0;
}
void showData(MYSQL *conn) {
	MYSQL_RES *res_ptr;						//儲存搜尋結果用的指標
	MYSQL_FIELD *field;						//儲存欄位資訊的指標
	MYSQL_ROW result_row;					//儲存每一行搜尋結果的值
	int row, column;						//分別儲存行和列的數量
	int i, j;								//迴圈用計算值

	res_ptr = mysql_store_result(conn);		//接收結果

	if(res_ptr) {							//判斷是否收到結果
		column = mysql_num_fields(res_ptr);	//接收列數
		row = mysql_num_rows(res_ptr);		//接收行數
		printf("Searched %d rows\n", row);	//列印搜尋到的行數

		for(i=0; (field = mysql_fetch_field(res_ptr)); i++) { //如果有回傳欄位資料的話 印出欄位的名稱
			printf("%10s", field->name);	
		}
		puts("\n");

		for(i=1; i<=row; i++) {				
			result_row = mysql_fetch_row(res_ptr); 		//在每次執行時 會自動接收下一行資料
			for(j=0; j<column; j++) {					//印出該行所有資料
				printf("%10s", result_row[j]);
			}
			puts("\n");
		}
	}
}

int commandCk(char *sqlCommand) {						//判斷使用者輸入是否為select 或 show (大小寫皆可)
	char select[] = "select";
	char SELECT[] = "SELECT";
	char show[] = "show";
	char SHOW[] = "SHOW";

	if(!(strncmp(select, sqlCommand, 6)) || !(strncmp(show, sqlCommand, 4)) || 
	!(strncmp(SELECT, sqlCommand, 6)) || !(strncmp(SHOW, sqlCommand, 4))) {  
		//判斷輸入是否符合select 或 show 是則回傳1 
		return 1;
	} 
	return 0;
}

