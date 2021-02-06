all: 
	

edit:
	gcc mysqldict.c -o mysqldict.out mysqllib.a -l mysqlclient

install:
	gcc -c mysqllib.c
	ar rcs mysqllib.a mysqllib.o

clean: 
	rm mysqldict.out