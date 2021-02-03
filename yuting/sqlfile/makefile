Sqlflag =-I mariadb/mysql.h  -lmariadb

all : csql.c
	gcc $^ $(Sqlflag) -o csql

clean : 
	@rm csql
