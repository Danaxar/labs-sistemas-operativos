// Daniel Catalán 20.675.871-6
// Felipe Carrasco 20.285.756-6

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>

// Entradas: argc y argv
// Salidas: int
// Descripción: Valida parametros de entrada e invoca a broker con execv
int main(int argc, char *argv[]){
    // Tienen que ser 5 o 6 argumentos (-b)
    if(!(argc == 10 || argc == 9)){
        printf("[Lab2] Argumentos insuficientes.\n");
        exit(1);
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

    if(c <= 0 || n <= 0){
        printf("Valores de chunk y workers invalidos.\n");
        exit(1);
    }

    // Crear Broker
    int pid = fork();
    if(pid == 0){
        // Convertir a char* el chunk
        char c_str[10];
        sprintf(c_str, "%d", c);

        // Convertir a char* la cantidad de workers
        char n_str[10];
        sprintf(n_str, "%d", n);

        // Crear lista de argumentos
        char* argumentos[] = {
            "./broker",
            i,
            o, 
            c_str, 
            n_str, 
            b ? "1" : "0",
            NULL};

        // Ejecutar broker
        execv(argumentos[0], argumentos);

        printf("No se ha podido ejecutar el worker\n");
        exit(1);
    }

    return 0;
}