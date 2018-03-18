
.DEFAULT_GOAL := all

calc : calc.h calc.c lexer.c syntax.c
	gcc -o $@ $^

.PHONY: all
all : calc

.PHONY: clean
clean :
	rm calc
