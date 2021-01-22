all:connect_sql.o
	 ./connect_sql.o

connect_sql.o:connect_sql.c
	 gcc -I usr/include/mysql connect_sql.c -L usr/lib/mysql -lmysqlclient -o connect_sql.o  
