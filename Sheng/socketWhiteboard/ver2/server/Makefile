CC = gcc
objs = ../src/linkedlist.c socketServer.c

all: $(objs) 
	gcc $^ -I ../include -o socketServer

.PHONY: clean
clean:
	-@rm socketServer
