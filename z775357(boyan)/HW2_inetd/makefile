FILENAME := sql
edit:
	vim $(FILENAME).c
all:
	gcc $(FILENAME).c -lmysqlclient -o $(FILENAME).out
clean:
	rm $(FILENAME).out
out:
	./$(FILENAME).out
