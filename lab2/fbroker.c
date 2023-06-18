#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>


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
    int** fd = (int**) malloc(sizeof(int* ) * n);
    for(int i = 0; i < n; i++){
        fd[i] = (int*) malloc(sizeof(int) * 2);
    }

    // Crear workers
    for(int i = 0; i < n; i++){
        // Crear pipe
        if(pipe(fd[i]) == -1){
            printf("[Broker] Error al crear el pipe\n");
            return 1;
        }

        // Crear un proceso hijo
        int pid = fork();
        if(pid == -1){
            printf("[Broker] Error al crear un nuevo proceso\n");
            return 1;
        }

        // Utilizar execv en el proceso hijo
        if (pid == 0){
            // Redirigir la salida estandar del proceso hijo hacia el padre
            dup2(STDOUT_FILENO, fd[i][1]);
            //! Ejecutar ./worker
            char* args[] = {"./worker", NULL};
            execv(args[0], args);

            printf("Error creando el worker\n");
            return 1;
        }
    }

    // Esperar a que los procesos hijos terminen
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    

    // // Leer salidas de los hijos desde los pipes
    // for(int i = 0; i < n; i++){
    //     int bytes_leidos = read(pipes[i][0], bd[i], sizeof(bd[i]));
    //     if(bytes_leidos > 0){
    //         printf("[Broker] [Hijo %d] %s", i, bd[i]);
    //     }

    //     // Cerrar la lectura
    //     close(pipes[i][0]);
    // }

    printf("[Broker] Finalizando broker... \n");
    return 0;
}
