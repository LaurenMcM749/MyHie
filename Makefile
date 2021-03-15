myhie: myhie.c sorter1.c sorter2.c 
	gcc myhie.c -o myhie
	gcc sorter1.c -o sorter1
	gcc sorter2.c -o sorter2

myhie.o: myhie.c myhie.h
	gcc -c myhie.c


clean:
	rm *.o myhie
