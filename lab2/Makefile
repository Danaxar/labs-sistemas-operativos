FLAGS= -Wall -lm

# Se definen los ejecutables a realizar
all: broker worker lab2

lab2: lab2.c
	gcc $(FLAGS) lab2.c -o lab2

broker: broker.o fbroker.o
	gcc $(FLAGS) -o broker broker.o fbroker.o

broker.o:
	gcc $(FLAGS) -c broker.c

fbroker.o:
	gcc $(FLAGS) -c fbroker.c

worker: worker.o fworker.o
	gcc $(FLAGS) -o worker worker.o fworker.o

worker.o:
	gcc $(FLAGS) -c worker.c

fworker.o:
	gcc $(FLAGS) -c fworker.c

clean:
	rm -f programa *.o