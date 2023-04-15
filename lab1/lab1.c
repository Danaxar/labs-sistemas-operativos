#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#define false 0
#define true 1
#define boolean int

/*
Entradas:
    - argc: Cantidad de argumentos + 1 (considera el nombre del ejecutable)
    - argv: Lista de argumentos
Salidas: Entero
*/
int main(int argc, char *argv[]){
    printf("Iniciando...\n");
    printf("Cantidad de argumentos: %d", argc);

    // Verificar cantidad de argumentos
    if(!(argc == 6 || argc == 5)){
        printf("Argumentos invalidos.\n");
        exit(0);
    }

    // Variables iniciales
    int option;
    char* i = NULL;
    char* o = NULL;
    boolean b = false;

    // Guardar los parametros de entrada
    while((option = getopt(argc, argv, "i:o:b")) != -1){
        switch (option){
        case 'i':
            // printf("option: %d, %c \n", option, option);
            i = (char*) malloc(strlen(optarg) + 1);
            strcpy(i, optarg);
            break;
        case 'o':
            // printf("option: %d, %c \n", option, option);
            o = (char*) malloc(strlen(optarg) + 1);
            strcpy(o, optarg);
            break;
        case 'b':
            // printf("option: %d\n", option);
            b = true;
            break;
        }
    }

    printf("Entradas del sistema:\n");
    printf("\ti: %s\n\to: %s\n", i, o);
    printf("\tb: %d\n", b);
    free(i);
    free(o);
    printf("Finalizando...\n");
    return 0;
}