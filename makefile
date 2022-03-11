shell: p3.o list.o memlist.o proclist.o
	gcc -Wall p3.o list.o memlist.o proclist.o -o shell
	rm -f p3.o list.o memlist.o proclist.o

p3.o: p3.c list.h memlist.h proclist.h 
	gcc -c -Wall p3.c

list.o: list.c list.h
	gcc -c -Wall list.c

memlist.o: memlist.c memlist.h
	gcc -c -Wall memlist.c

proclist.o: proclist.c proclist.h
	gcc -c -Wall proclist.c
