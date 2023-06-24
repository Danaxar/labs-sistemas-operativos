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
    // printf("[Worker %s] Leyendo mensaje de padre...\n", id);
    if(read(fd_read, buffer, BUFF_SIZE) == -1){
        printf("[Worker %s] Error al leer el mensaje del padre\n", id);
        return 0;
    }
    // print("Mensaje recibido: %s", buffer);
    return 1;
}

int enviarMensaje(char* msj){
    // printf("[Worker %s] Enviando mensaje de padre...\n", id);
    if(write(fd_write, msj, BUFF_SIZE) == -1){
        printf("[Worker %s] Error escribir al padre\n", id);
        return 0;
    }
    // printf("[Worker %s] Envio existoso\n", id);
    return 1;
}


char* intArrayToString(int* array, int length) {
    int resultLength = length + 4;  // Suponiendo que cada número ocupa como máximo 12 caracteres
    
    // Crea la cadena de caracteres resultante
    char* result = (char*)malloc((resultLength + 1) * sizeof(char));
    if (result == NULL) {
        // Manejo del error de asignación de memoria
        fprintf(stderr, "Error de asignación de memoria\n");
        return NULL;
    }
    
    // Convierte los enteros a caracteres y los agrega a la cadena resultante
    int i = 0;
    while(i < length){
        sprintf(&result[i], "%d", array[i]);
        i++;
    }
    print("Cadena resultante: %s\n", result);
    return result;
}


// Se recibe como entrada los descriptores de archivo
int main(int argc, char const *argv[])
{
    id = argv[1];
    fd_read = atoi(argv[2]);
    fd_write = atoi(argv[3]);
    int chunk = atoi(argv[4]);
    // print("Iniciando -> Argumentos: %d, %d", fd_read, fd_write);

    char entrada[BUFF_SIZE];
    int* respuestas = (int*) malloc(sizeof(int) * chunk);
    int lineasLeidas = 0;
    while(1){
        leerMensaje(entrada);
        if(strcmp(entrada, "quit") == 0){
            // Enviar data
            // print("Lineas leidas: %d", lineasLeidas);

            // Convertir lista de enteros en string
            char* salida = intArrayToString(respuestas, chunk);

            //! Cada worker tiene una capacidad máxima de 255 bytes se info
            enviarMensaje(salida);
            // enviarMensaje("1111111111");
            break;
        }else{
            // Procesar mensaje (se asume que no hay errores)
            int resultado;
            estado1(&resultado, entrada, 0);
            // printf("[Worker %s] Linea recibida: %s -> %d\n",id, entrada, resultado);
            printf("\r");
            lineasLeidas++;

            int decision = resultado == 4 ? 1 : 0;
            respuestas[lineasLeidas] = decision; // Guardar resultado
        }

    }

    // print("Finalizando...\n");
    return 0;
}
