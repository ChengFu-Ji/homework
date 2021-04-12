all:showN.o
	echo Compiler Done

run:showN.o
	./showN.o test1.txt test2.txt

showN.o:showN.c
	gcc showN.c -o showN.o

.PHYON:clean

clean:showN.o
	rm showN.o
