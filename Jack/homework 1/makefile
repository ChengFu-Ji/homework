file:=sql.out
all:sql.c
        gcc sql.c -o $(file) -I usr/include/mysql -L /usr/lib/mysql -lmysqlclient
        ./sql.out
clean:
        rm -f $(file)
