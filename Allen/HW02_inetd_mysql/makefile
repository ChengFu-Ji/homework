all: 
	gcc mysqldict.c -o mysqldict.out mysqllib.a -l mysqlclient

edit:
	ar rcs mysqllib.a mysqllib.o

clean: 
	rm mysqldict.out