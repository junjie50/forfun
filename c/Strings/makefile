replace: replace.o strp.o
	gcc replace.o strp.o -o replace

replace.o: replace.c str.h
	gcc -c replace.c

teststr: teststr.o strp.o
	gcc teststr.o strp.o -o teststr

teststr.o: teststr.c str.h
	gcc -c teststr.c

strp.o: strp.c str.h
	gcc -c strp.c

