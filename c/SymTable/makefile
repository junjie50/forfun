testsymtable: testsymtable.o symtablehash.o
	gcc testsymtable.o symtablehash.o -o testsymtable

testsymtable.o: testsymtable.c symtable.h
	gcc -c testsymtable.c

symtablehash.o: symtablehash.c symtable.h 
	gcc -c symtablehash.c	

symtablelist.o: symtablelist.c symtable.h 
	gcc -c symtablelist.c