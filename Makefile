shell : shell.c
	gcc -c shell.c
	gcc -c linkedList.c
	gcc -o shell shell.o linkedList.o

clean:
	rm -f shell
	rm -f shell.o
	rm -f linkedList.o
