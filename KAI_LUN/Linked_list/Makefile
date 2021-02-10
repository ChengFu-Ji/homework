
OUT_FILE = linklist.o
COMPILER_FILE = linklist.c

all :$(OUT_FILE) 
	./$(OUT_FILE)

$(OUT_FILE) : $(COMPILER_FILE) 
	gcc $(COMPILER_FILE) -o $(OUT_FILE)

.PHONY:clean
clean:$(OUT_FILE)
	rm $(OUT_FILE)
