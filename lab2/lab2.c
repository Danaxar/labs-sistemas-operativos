// Daniel Catalán 20.675.871-6
// Felipe Carrasco 20.285.756-6

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include "funciones.h"

// Entradas: argc y argv
// Salidas: int
// Descripción: Analiza un archivo de entrada y escribe otro con los resultados de las expresiones regulares
int main(int argc, char *argv[]){
    printf("[Lab2] Iniciando lab2...\n");
    // Tienen que ser 5 o 6 argumentos
    if(!(argc == 10 || argc == 9)){
        printf("[Lab2] Argumentos invalidos.\n");
        exit(0);
    }

    // Getopt
    char* i = NULL;
    char* o = NULL;  // Nombres de entrada y salida de ficheros
    int c; // Tamaño del chunk, cantidad de lineas a leer por chunk
    int n; // Cantidad de workers a generar
    int b; // Flag: Cuando se activa muestra por pantalla los resultados
    b = 0;

    int option;
    while((option = getopt(argc, argv, "i:o:c:n:b")) != -1){
        switch (option){
            case 'i':
                i = (char*) malloc(strlen(optarg) + 1);
                strcpy(i, optarg);
                break;
            case 'o':
                o = (char*) malloc(strlen(optarg) + 1);
                strcpy(o, optarg);
                break;
            case 'c':
                c = atoi(optarg);
                break;
            case 'n':
                n = atoi(optarg);
                break;
            case 'b':
                b = 1;
                break;
        }
    }


    // printf("[Lab2] \ti: %s\n", i);
    // printf("[Lab2] \to: %s\n", o);
    // printf("[Lab2] \tc: %d\n", c);
    // printf("[Lab2] \tn: %d\n", n);
    // printf("[Lab2] \tb: %d\n", b);
    b ? printf("\r") : 0;

    // Crear Broker
    int pid = fork();
    if(pid == 0){  // Hijo -> Ejecutar broker
        // Convertir c y n a cadena de caracteres
        char c_str[10];
        char n_str[10];
        sprintf(c_str, "%d", c);
        sprintf(n_str, "%d", n);
        char* argumentos[] = {"./broker", "-c", c_str, "-n", n_str, NULL};

        // Ejecutar broker
        execv(argumentos[0], argumentos);
        // El broker creará ${n} workers
        // El broker ejecutará los procesos worker utilizando algún miembro de la familia exec()
        // Y se comunicará con ellos mediante el uso de pipes
    }

    printf("[Lab2] Finalizando...\n");
    free(i);
    free(o);
    return 0;
}