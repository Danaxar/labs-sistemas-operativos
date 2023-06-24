#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFF_SIZE 255
#define print(...) printf("[Broker] "); printf(__VA_ARGS__); printf("\n")
#define MAX_CHAR 60
#define tab printf("\n");

// Funciones
void mandarMensaje(int fdHijo, char* contenido){
    write(fdHijo, contenido, BUFF_SIZE);
}

void leerMensaje(int fdHijo, char* buff){
    if(read(fdHijo, buff, BUFF_SIZE) == -1){
        print("Error al leer el mensaje del hijo");
    }
}

// Entradas: char* (nombre del archivo)
// Salidas: int (cantidad de lineas del archivo)
// Descripción: Obtiene la cantidad de lineas del nombre del archivo después del separador "_"
// Ej: prueba_10.txt -> 10, prueba_100.txt = 100
int getCantidadLineas(char* nombreArchivo){
    char* posicionSeparador = strchr(nombreArchivo, '_') + 1;
    return atoi(posicionSeparador);
}

// Entrada: char* (nombre del archivo a leer con extensión), int* (cantidad de lineas)
// Salida: char** (Matriz de cadena de caracteres del contenido del archivo)
// Descripción: Lee un archivo de principio a fin
char** leerArchivo(char* nombreArchivo, int* cantidadLineas){
    print("Nombre del archivo: %s", nombreArchivo);
    // En el enunciado se muestran 60 caracteres de prueba
    int lenLinea = MAX_CHAR;

    FILE* archivo = fopen(nombreArchivo, "r");
    if(archivo == NULL){
        printf("No se ha podido leer el archivo\n");
        exit(1);
    }

    *cantidadLineas = getCantidadLineas(nombreArchivo);

    // Leer
    char** salida = (char**) malloc(sizeof(char*) * *cantidadLineas);
    for(int i = 0; i < *cantidadLineas; i++){
        salida[i] = (char*) malloc(sizeof(char) * lenLinea);
        fscanf(archivo, "%s", salida[i]);
    }

    fclose(archivo);
    return salida;
}

// Cantidad de lineas a leer: c * n
int main(int argc, char *argv[]) {
    print("Iniciando broker...");
    int c = 0; // Tamaño del chunk
    int n = 0; // Cantidad de workers
    char* archivo_entrada = "prueba_100.txt";
    char* o = "salida.txt";

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
        print("Debe escoger un numero distinto de cero.");
        return 1;
    }

    print("Tamaño del chunk: %d", c);
    print("Cantidad de workers: %d", n);
    

    // Resultados workers

    // Leer archivo
    int cantidadLineas;
    char** archivo = leerArchivo(archivo_entrada, &cantidadLineas);
    // Mostrar por pantalla el archivo leido
    // for(int i = 0; i < cantidadLineas; i++) {print("%s", archivo[i]);}


    // Adjudicar memoria para los pipes
    // Hijo -> Padre (Hijo escribe padre lee)
    int** fd_parent_child = (int**) malloc(sizeof(int* ) * n);
    for(int i = 0; i < n; i++) {fd_parent_child[i] = (int*) malloc(sizeof(int) * 2);}

    // Padre -> Hijo (Padre escribe hijo lee)
    int** fd_child_parent = (int**) malloc(sizeof(int* ) * n);
    for(int i = 0; i < n; i++) {fd_child_parent[i] = (int*) malloc(sizeof(int) * 2);}


    // Crear workers
    print("Creando workers...");
    for(int i = 0; i < n; i++){
        // Crear pipe
        if(pipe(fd_parent_child[i]) == -1 || pipe(fd_child_parent[i]) == -1){
            print("Error al crear el pipe");
            return 1;
        }

        // Crear un proceso hijo
        int pid = fork();
        if(pid == -1){
            print("Error al crear un nuevo proceso");
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

            char chunk_str[BUFF_SIZE];
            sprintf(chunk_str, "%d", c);

            char* args[] = {"./worker", idWorker,fd_read, fd_write, chunk_str, NULL};

            // Ejecutar worker (Se sale del ciclo)
            execv(args[0], args);

            // Si esta parte se ejecuta entonces execv salió mal
            print("Error creando el worker");
            return 1;
        }
    }
    print("Workers creados.");

    // Una vez creados hay que mandar lineas para procesar
    // Cada worker leerá 1 chunk (X lineas)
    print("Mandando lineas a workers...");
    int line = 0;
    int archivoLeido = 0;
    // Recorrer workers
    for(int w = 0; w < n && !archivoLeido; w++){
        // Recorrer lineas relativas y mandarlas al worker
        for(int l = 0; l < c; l++){
            if(line + l < cantidadLineas){
                // print("Mensaje a enviar: %s\n", archivo[line + l]);
                mandarMensaje(fd_parent_child[w][1],archivo[line + l]);
            }else{
                // Se llegó al límite del archivo
                break;
                archivoLeido = 1;
            }
        }
        line += c;
    }

    // // int contadorLineasLeidas = 0;
    // // Escribir archivo de salida
    // FILE* salida = fopen(o, "w");
    // line = 0;
    // // Recorrer workers
    // for(int w = 0; w < n; w++){
    //     print("Leyendo al worker %d", w);
    //     char mensaje[1000];
    //     print("XD1");
    //     // leerMensaje(fd_child_parent[w][0], mensaje);
    //     print("Fd: %d", fd_child_parent[w][0]);
    //     read(fd_child_parent[w][0], mensaje, 1000);
    //     print("XD2");
    //     tab print("Linea: %s", mensaje);
        

    //     // Recorrer resultados del worker
    //     for(int result = 0; result < strlen(mensaje); result++){
    //         // Imprimir linea
    //         fprintf(salida, "%s ", archivo[line + result]);  

    //         // Imprimir resultado
    //         fprintf(
    //             salida,
    //             "%s\n",
    //             // strcmp(mensaje[result], "1") == 0 
    //             mensaje[result] == '1'
    //                 ? "si\n"
    //                 : "no\n"
    //         );
    //     }
    //     line += c;
    // }
    // fclose(salida);


    // Mandar mensajes quit
    print("Mandando mensajes quit...");
    for(int i = 0; i < n; i++){
        mandarMensaje(fd_parent_child[i][1], "quit");
    }

    // Esperar a que los procesos hijos terminen
    print("Esperando a que los workers terminen...");
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    // Cerrar los pipes
    print("Cerrando los pipes...");
    for(int i = 0; i < n; i++){
        close(fd_parent_child[i][0]);
        close(fd_parent_child[i][1]);
    }

    print("Finalizando broker... \n");
    return 0;
}
