OUTFILE = connect_sql.o
INCLUDE_MYSQL = usr/include/mysql
LIB_MYSQL =  usr/lib/mysql
MY_SQL_CLIENT = -lmysqlclient
RUN = ./

all:$(OUTFILE)
	$(RUN)$(OUTFILE)

connect_sql.o:connect_sql.c
	 gcc -I $(INCLUDE_MYSQL) connect_sql.c -L $(LIB_MYSQL) -lmysqlclient -o  $(OUTFILE) 

clean:
	rm -f $(OUTFILE)
