CC = gcc
config:= `mysql_config --libs --include`

all: sqlConnect.c
	$(CC) $^ $(config) -o sqlConnect

.PHONY: clean
clean:
	-@rm sqlConnect
