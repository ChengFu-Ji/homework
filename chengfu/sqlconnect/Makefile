CC=gcc
LIB=-I/usr/include/mysql -lmariadb
CFLAG=-O3

all: main.c
		$(CC) $^ $(CFLAG) $(LIB) -o connect 

debug: main.c
		$(CC) $^ $(CFLAG) $(LIB) -DDEBUG -o connect 

clean:
		@rm connect
