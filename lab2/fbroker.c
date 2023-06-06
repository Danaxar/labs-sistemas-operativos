#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    printf("Iniciando broker...\n");
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
        printf("Debe escoger un numero distinto de cero.\n");
        return 1;
    }

    printf("Tamaño del chunk: %d\n", c);
    printf("Cantidad de workers: %d\n", n);

    printf("Creando workers...\n");
    for(int i = 0; i < n; i++){
        // El broker creará ${n} workers
        int pid = fork();
        if (pid == 0){
            // El broker ejecutará los procesos worker utilizando algún miembro de la familia exec()
            //! Ejecutar ./worker


            // Y se comunicará con ellos mediante el uso de pipes
            break;
        }
    }

    printf("Finalizando broker... \n");
    return 0;
}
