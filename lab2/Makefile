FLAGS= -Wall -lm

# Se definen los ejecutables a realizar
all: broker worker lab2

broker: broker.c
	gcc $(FLAGS) broker.c -o broker

worker: worker.c fworker.c
	gcc $(FLAGS) worker.c fworker.c -o worker

lab2: lab2.c
	gcc $(FLAGS) lab2.c -o lab2

funciones.o:
	gcc $(FLAGS) funciones.c -c

clean:
	rm -f programa *.o