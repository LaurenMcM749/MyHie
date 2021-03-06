myhie: myhie.c
	gcc myhie.c -o myhie

myhie.o: myhie.c myhie.h
	gcc -c myhie.c

clean:
	rm *.o myhie
