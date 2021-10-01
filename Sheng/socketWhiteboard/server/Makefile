CC = gcc
objs = linkedlist.c socketServer.c

all: $(objs) 
	$(CC) $^ -I ../include -o linkedlist 

.PHONY: clean
clean:
	-@rm linkedlist
