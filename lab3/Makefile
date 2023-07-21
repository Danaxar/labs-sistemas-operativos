FLAGS = -Wall -lm

all: lab3

lab3: lab3.o funciones.o
	gcc $(FLAGS) -o lab3 lab3.o funciones.o

lab3.o: lab3.c
	gcc $(FLAGS) -c lab3.c

funciones.o: funciones.c
	gcc $(FLAGS) -c funciones.c

clean:
	rm -f *.o
