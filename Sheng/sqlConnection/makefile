config:= `mysql_config --libs --include`
obj:= sqlConnect.o

edit: $(obj)
	gcc -o $@ $< $(config)
$(obj):

.PHONY: clean
clean:
	-rm edit $(obj)
