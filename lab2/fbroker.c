#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFF_SIZE 255

// Cantidad de lineas a leer: c * n

int main(int argc, char *argv[]) {
    printf("[Broker] Iniciando broker...\n");
    int c = 0; // Tamaño del chunk
    int n = 0; // Cantidad de workers

    int option;
    while ((option = getopt(argc, argv, "c:n:")) != -1) {
        switch (option) {
            case 'c':
                c = atoi(optarg);
                break;
            case 'n':
                n = atoi(optarg);
                break;
        }
    }

    if (c == 0 || n == 0) {
        printf("[Broker] Debe escoger un numero distinto de cero.\n");
        return 1;
    }

    printf("[Broker] Tamaño del chunk: %d\n", c);
    printf("[Broker] Cantidad de workers: %d\n", n);
    printf("[Broker] Creando workers...\n");

    // Matriz donde se almacenará lo que se lea desde los workers
    int cantidadLineas = c * n;
    int cantidadCol = 80;
    char** bd = (char**) malloc(sizeof(char* ) * cantidadLineas);
    for(int i = 0; i < cantidadLineas; i++){
        bd[i] = (char*) malloc(sizeof(char) * cantidadCol);
    }

    // Adjudicar memoria para los pipes
    // Hijo -> Padre (Hijo escribe padre lee)
    int** fd_parent_child = (int**) malloc(sizeof(int* ) * n);
    for(int i = 0; i < n; i++){
        fd_parent_child[i] = (int*) malloc(sizeof(int) * 2);
    }

    // Padre -> Hijo (Padre escribe hijo lee)
    int** fd_child_parent = (int**) malloc(sizeof(int* ) * n);
    for(int i = 0; i < n; i++){
        fd_child_parent[i] = (int*) malloc(sizeof(int) * 2);
    }

    // Crear workers
    for(int i = 0; i < n; i++){
        // Crear pipe
        if(pipe(fd_parent_child[i]) == -1 || pipe(fd_child_parent[i]) == -1){
            printf("[Broker] Error al crear el pipe\n");
            return 1;
        }

        // Crear un proceso hijo
        int pid = fork();
        if(pid == -1){
            printf("[Broker] Error al crear un nuevo proceso\n");
            return 1;
        }

        if (pid == 0){
            // Id del worker
            char idWorker[32]; 
            sprintf(idWorker, "%d", i);

            // FD para lectura
            char fd_read[BUFF_SIZE];
            sprintf(fd_read, "%d", fd_parent_child[i][0]);

            // Canal de escritura del hijo
            char fd_write[BUFF_SIZE];
            sprintf(fd_write, "%d", fd_child_parent[i][1]);

            char* args[] = {"./worker", idWorker,fd_read, fd_write, NULL};

            // Ejecutar worker (Se sale del ciclo)
            execv(args[0], args);

            // Si esta parte se ejecuta entonces execv salió mal
            printf("Error creando el worker\n");
            return 1;
        }
    }
    printf("[Broker] Workers creados.\n");

    // Enviar un mensaje al primer hijo
    write(fd_parent_child[0][1], "Padre->Hijo", 12);

    // Leer un mensaje del primer hijo
    char mensajeHijo[1000];
    if(read(fd_child_parent[0][0], mensajeHijo, BUFF_SIZE) == -1){
        printf("[Broker] Error al leer el mensaje del hijo\n");
    }else{
        printf("[Broker] Mensaje recibido por hijo: %s\n", mensajeHijo);
    }

    


    

    // Esperar a que los procesos hijos terminen
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    // Cerrar los pipes
    // for(int i = 0; i < n; i++){
    //     close(fd_parent_child[i][0]);
    //     close(fd_parent_child[i][1]);
    // }

    printf("[Broker] Finalizando broker... \n");
    return 0;
}
