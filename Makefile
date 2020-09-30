smtp: client.o smtp.o
	gcc -o client.out client.o smtp.o

client.o: client.c
	gcc -c client.c

smtp.o: smtp.c smtp.h
	gcc -c smtp.c

clean:
	rm -f *.o *.out
