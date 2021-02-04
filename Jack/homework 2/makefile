file:=sql.out
lib:=-I usr/include/mysql -L /usr/lib/mysql -lmysqlclient
all:sql.c
	gcc sql.c -o $(file) $(lib)
	./sql.out
clean:
	rm -f $(file)
