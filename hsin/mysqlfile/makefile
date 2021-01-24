#make ->執行
#make clean->刪除執行檔(mysqltest)

test = mysqltest

all:$(test)		
	./$(test)

mysqltest: mysqltest.c
	gcc $^ -o $(test) -lmysqlclient

clean:				
	rm mysqltest
 
