CC=gcc
#LIB=-I/usr/include/mysql -lmariadb
CFLAG=-I mariadb/mysql.h  -lmariadb

all: main.c
	$(CC) $^ $(CFLAG)  -o connect 

debug: main.c
	$(CC) $^ $(CFLAG)  -DDEBUG -o connect 

clean:
	@rm connect
