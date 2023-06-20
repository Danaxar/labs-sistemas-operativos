#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFF_SIZE 255

// Variables globales
const char* id;
int fd_read, fd_write;

int leerMensaje(char* buffer){
    printf("[Worker %s] Leyendo mensaje de padre...\n", id);
    if(read(fd_read, buffer, BUFF_SIZE) == -1){
        printf("[Worker %s] Error al leer el mensaje del padre\n", id);
        return 0;
    }
    return 1;
}

int enviarMensaje(char* msj){
    printf("[Worker %s] Enviando mensaje de padre...\n", id);
    if(write(fd_write, msj, BUFF_SIZE) == -1){
        printf("[Worker %s] Error escribir al padre\n", id);
        return 0;
    }
    printf("[Worker %s] Envio existoso\n", id);
    return 1;
}

// Se recibe como entrada los descriptores de archivo
int main(int argc, char const *argv[])
{
    id = argv[1];
    printf("\n[Worker %s] Iniciando...\n", id);
    fd_read = atoi(argv[2]);
    fd_write = atoi(argv[3]);
    printf("[Worker %s] Argumentos: %d, %d\n", id, fd_read, fd_write);

    char bd[BUFF_SIZE];
    leerMensaje(bd) ? printf("[worker %s]\t Recibido: %s\n", id, bd) : 0;

    char mensaje[BUFF_SIZE] = "Hijo->Padre";
    enviarMensaje(mensaje);
    close(fd_write);


    printf("[Worker %s] Finalizando...\n\n", id);
    return 0;
}
