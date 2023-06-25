#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fworker.h"

#define BUFF_SIZE 255
#define print(...) printf("[Worker %s] ", id); printf(__VA_ARGS__); printf("\033[0m\n");

// Variables globales
const char* id;
int fd_read, fd_write;

int leerMensaje(char* buffer){
    read(fd_read, buffer, BUFF_SIZE);
    return 1;
}

int enviarMensaje(char* msj){
    if(write(fd_write, msj, BUFF_SIZE) == -1){
        printf("[Worker %s] Error escribir al padre\n", id);
        return 0;
    }
    return 1;
}


char* intArrayToString(int* array, int len) {
    // Crea la cadena de caracteres resultante
    char* salida = (char*)malloc((len + 1) * sizeof(char));
    int i;
    for(i = 0; i < len; i++) sprintf(&salida[i], "%d", array[i]);
    salida[i] = '\0';
    printf("toString -> %s\n", salida);
    return salida;
}


// Se recibe como entrada los descriptores de archivo
int main(int argc, char const *argv[])
{
    id = argv[1];
    fd_read = atoi(argv[2]);
    fd_write = atoi(argv[3]);
    int chunk = atoi(argv[4]);
    printf("\r%d\r", chunk); //! Borrar

    char entrada[BUFF_SIZE];
    int lineasLeidas = 0;
    while(1){
        leerMensaje(entrada);
        if(strcmp(entrada, "FIN") == 0){
            char lineasLeidas_str[BUFF_SIZE];
            sprintf(lineasLeidas_str, "%d", lineasLeidas);
            break;
        }else{
            int resultado;
            estado1(&resultado, entrada, 0);
            printf("\r");  //? Si quito este print se cae el programa ._.
            lineasLeidas++;

            // Escribir en el pipe
            write(fd_write, resultado == 4 ? "1" : "0",1);
        }
    }

    return 0;
}
