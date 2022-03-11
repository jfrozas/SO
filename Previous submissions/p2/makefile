shell: p2.o list.o memlist.o
	gcc p2.o list.o memlist.o -o shell

p2.o: p2.c list.h
	gcc -c p2.c

list.o: list.c list.h
	gcc -c list.c

memlist.o: memlist.c memlist.h
	gcc -c memlist.c
