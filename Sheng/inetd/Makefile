CC = gcc
config:= `mysql_config --libs --include`

all: myservice.c
	$(CC) $^ $(config) -o myinetdtest 

.PHONY: clean
clean:
	-@rm myinetdtest
