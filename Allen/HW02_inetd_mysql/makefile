all: 
	gcc -c mysqllib.c
	ar rcs mysqllib.a mysqllib.o
	gcc mysqldict.c -o mysqldict.out mysqllib.a -l mysqlclient

clean: 
	rm mysqldict.out