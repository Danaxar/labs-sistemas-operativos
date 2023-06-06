FLAGS= -Wall -lm

# Se definen los ejecutables a realizar
all: broker worker lab2

broker: fbroker.c
	gcc $(FLAGS) fbroker.c -o broker

worker: fworker.c
	gcc $(FLAGS) fworker.c -o worker

lab2: lab2.c funciones.o
	gcc $(FLAGS) lab2.c funciones.o -o lab2

funciones.o:
	gcc $(FLAGS) funciones.c -c

clean:
	rm -f programa *.o