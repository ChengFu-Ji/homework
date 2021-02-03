CC = gcc
config:= `mysql_config --libs --include`

all: inetd_daemons.c
	$(CC) $^ $(config) -o myinetdtest 

.PHONY: clean
clean:
	-@rm myinetdtest
