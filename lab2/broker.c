#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFF_SIZE 255
#define print(...) printf("\033[0;36m[Broker] "); printf(__VA_ARGS__); printf("\033[0m\n")
#define MAX_CHAR 60
#define tab printf("\t");
int b;

// Funciones
void mandarMensaje(int fdHijo, char* contenido){
    write(fdHijo, contenido, BUFF_SIZE);
}

void leerMensaje(int fdHijo, char* buff){
    read(fdHijo, buff, BUFF_SIZE);
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

int main(int argc, char *argv[]) {
    // Parametros de entrada
    char* nombreArchivoEntrada = argv[1];
    char* nombreArchivoSalida = argv[2];
    int chunk = atoi(argv[3]);
    int n = atoi(argv[4]); // Cantidad de workers
    b = strcmp(argv[5], "1") == 0 ? 1 : 0;  // Flag b

    // Leer archivo de entrada
    int cantidadLineas;
    char** archivo = leerArchivo(nombreArchivoEntrada, &cantidadLineas);

    // Adjudicar memoria para los pipes
    int** fd_parent_child = (int**) malloc(sizeof(int* ) * n); // Hijo escribe padre lee
    int** fd_child_parent = (int**) malloc(sizeof(int* ) * n); // Padre escribe hijo lee
    for(int i = 0; i < n; i++) {
        fd_parent_child[i] = (int*) malloc(sizeof(int) * 2);
        fd_child_parent[i] = (int*) malloc(sizeof(int) * 2);
    }

    // Crear workers
    for(int i = 0; i < n; i++){
        // Crear pipe
        if(pipe(fd_parent_child[i]) == -1 || pipe(fd_child_parent[i]) == -1){
            print("Error al crear el pipe");
            exit(1);
        }

        // Crear un proceso hijo
        int pid = fork();
        if(pid == -1){
            print("Error al crear un nuevo proceso");
            exit(1);
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

            // Tamaño del chunk
            char chunk_str[BUFF_SIZE];
            sprintf(chunk_str, "%d", chunk);

            // Argumentos
            char* args[] = {"./worker", idWorker,fd_read, fd_write, chunk_str, NULL};

            // Ejecutar worker
            execv(args[0], args);

            print("Error creando el worker");
            exit(1);
        }
    }

    // Algoritmo
    int line = 0;
    int si = 0;
    int no = 0;
    FILE* salida = fopen(nombreArchivoSalida, "w");
    b ? printf("\n\n") : 0;
    while(line < cantidadLineas){
        int worker = (line / chunk) % n;
        fprintf(salida, "%s ", archivo[line]);
        b ? printf("%s ", archivo[line]) : 0;

        // Mandar linea a procesar
        mandarMensaje(fd_parent_child[worker][1], archivo[line]);
        
        // Recibir respuesta
        char recibir[BUFF_SIZE];
        leerMensaje(fd_child_parent[worker][0], recibir);

        int respuesta = atoi(recibir);
        respuesta ? si++: no++;

        fprintf(salida, "%s\n", respuesta ? "si" : "no");
        b ? printf("%s\n", respuesta ? "si" : "no") : 0;
        line++;
    }

    fprintf(salida,"\n\nTotal de expresiones que Si son regulares: %d\n", si);
    fprintf(salida,"Total de expresiones que No son regulares: %d\n", no);
    fprintf(salida,"Total de lineas leidas: %d\n", line);
    fclose(salida);
    
    // Lineas leidas
    if(b){
        print("Total de expresiones que Si son regulares: %d", si);
        print("Total de expresiones que No son regulares: %d", no);
        print("Total de lineas leidas: %d", cantidadLineas);
    }

    // Mandar mensajes FIN
    for (int i = 0; i < n; i++) mandarMensaje(fd_parent_child[i][1], "FIN");

    // Recibir lineas leidas
    for(int i = 0; i < n; i++){
        char recibir[BUFF_SIZE];
        leerMensaje(fd_child_parent[i][0], recibir);
        b ? printf("[Worker %d] Lineas leidas: %s\n", i, recibir) : 0;
    }

    // Esperar a que terminen los worker
    for (int i = 0; i < n; i++) wait(NULL);

    // Cerrar los pipes
    for(int i = 0; i < n; i++){
        close(fd_parent_child[i][0]);
        close(fd_parent_child[i][1]);
    }
    b ? printf("Fin\n") : 0;
    return 0;
}
