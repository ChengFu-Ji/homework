all: mysql_c_connect.out
	./mysql_c_connect.out

mysql_c_connect.out: mysql_c_connect.c
	gcc mysql_c_connect.c -o mysql_c_connect.out -l mysqlclient

clean:
	rm mysql_c_connect.out