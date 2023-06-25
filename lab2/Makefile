FLAGS = -Wall -lm

# Objetivo por defecto
all: broker worker lab2

lab2: lab2.c
	gcc $(FLAGS) lab2.c -o lab2

broker: broker.o fbroker.o
	gcc $(FLAGS) -o broker broker.o fbroker.o

broker.o: broker.c
	gcc $(FLAGS) -c broker.c

fbroker.o: fbroker.c
	gcc $(FLAGS) -c fbroker.c

worker: worker.o fworker.o
	gcc $(FLAGS) -o worker worker.o fworker.o

worker.o: worker.c
	gcc $(FLAGS) -c worker.c

fworker.o: fworker.c
	gcc $(FLAGS) -c fworker.c

clean:
	rm -f broker worker lab2 *.o
