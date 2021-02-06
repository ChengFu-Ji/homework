all: 
	gcc mysqldict.c -o mysqldict.out mysqllib.a -l mysqlclient

edit:
	gcc -c mysqllib.c
	ar rcs mysqllib.a mysqllib.o

clean: 
	rm mysqldict.out