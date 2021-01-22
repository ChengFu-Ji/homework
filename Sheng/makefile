config:= `mysql_config --libs --include`

edit: sqlConnect.o 
	gcc -o $@ $< $(config)
sqlConnect.o: sqlConnect.c
	gcc -c $< 

.PHONY: clean
clean:
	-rm edit sqlConnect.o
