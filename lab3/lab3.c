// Daniel Catalán 20.675.871-6
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define print(...) printf(__VA_ARGS__); printf("\n")
#define MAX_CHAR 60

// Cabeceras
void estado1(int* estadoActual, char* expresion, int indiceActual);
void estado2(int* estadoActual, char* expresion, int indiceActual);
void estado3(int* estadoActual, char* expresion, int indiceActual);
void estado4(int* estadoActual, char* expresion, int indiceActual);
int getCantidadLineas(char* nombreArchivo);
char** leerArchivo(char* nombreArchivo, int* cantidadLineas);
int threadByLine(int line);
void* hebra(void* arg);


// Variables globales
int c; // Tamaño del chunk, cantidad de lineas a leer por chunk
int n; // Cantidad de hebras a generar
char** entrada;  // Matriz de caracteres donde se guarda la información del archivo
pthread_mutex_t entrada_sem;
int* salida;  // Array de enteros donde se almacena el resultado de la linea
pthread_mutex_t salida_sem;
int cantidadLineas;  // Almacena la cantidad de lineas leidas


// Entradas: argc y argv
// Salidas: int
// Descripción: Valida parametros de entrada
int main(int argc, char *argv[]){
    // Tienen que ser 5 o 6 argumentos (-b)
    if(!(argc == 10 || argc == 9)){
        printf("[Lab2] Argumentos insuficientes.\n");
        exit(1);
    }

    // Getopt
    char* i = NULL;  // Nombre fichero entrada
    char* o = NULL;  // Nombre fichero salida
    int b = 0;       // Flag: Cuando se activa muestra por pantalla los resultados

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
        printf("Valores de chunk y hebras invalidos.\n");
        exit(1);
    }

    // print("Archivo de entrada: %s", i);
    // print("Archivo de salida: %s", o);
    // print("Mostrar salida? %s", b ? "si":"no");
    // print("Tamaño del chunk: %d", c);
    // print("Cantidad de hebras: %d", n);

    pthread_mutex_init(&entrada_sem, NULL);
    pthread_mutex_init(&salida_sem, NULL);

    // Leer archivo
    entrada = leerArchivo(i, &cantidadLineas);
    salida = (int*) malloc(sizeof(int) * cantidadLineas);


    // Crear las hebras
    pthread_t* idsHebras = (pthread_t*) malloc(sizeof(pthread_t) * n);
    int* codigosHebras = (int*) malloc(sizeof(int) * n);
    for(int i = 0; i < n; i++){
        codigosHebras[i] = i;
        pthread_create(&idsHebras[i], NULL, hebra, (void*) &codigosHebras[i]);
    }

    

    // Esperar a que terminen
    for(int i = 0; i < n; i++){
        pthread_join(idsHebras[i], NULL);
    }
    
    // Escribir el archivo de salida
    FILE* archivo_salida = fopen(o, "w");

    // Contar resultados y mostrar resultados
    int si_count = 0;
    int no_count = 0;
    for(int i = 0; i < cantidadLineas; i++){
        b ? printf("%s %s\n", entrada[i], salida[i] == 4 ? "si" : "no") : 0;
        fprintf(archivo_salida, "%s  %s\n", entrada[i], salida[i] == 4 ? "si" : "no");
        salida[i] == 4 ? si_count++:no_count++;
        free(entrada[i]);
    }
    fprintf(archivo_salida, "\n\n");

    fprintf(archivo_salida, "Total de expresiones que Si son regulares: %d\n", si_count);
    fprintf(archivo_salida, "Total de expresiones que No son regulares: %d\n", no_count);

    if(b){
        print("Total de expresiones que Si son regulares: %d", si_count);
        print("Total de expresiones que No son regulares: %d", no_count);
    }
    fclose(archivo_salida);
    free(entrada);
    free(salida);
    return 0;
}




// --------------------- ESTADOS ------------------------ //
// Entradas: int* (estado actual), char* (expresión a evaluar), int (numero de caracter a analizar)
// Salidas: ninguna
// Descripción: Representa el estado 1 del autómata finito
void estado1(int* estadoActual, char* expresion, int indiceActual){
    if(indiceActual == MAX_CHAR)
        return;

    *estadoActual = 1;

    if(expresion[indiceActual] == 'A' || expresion[indiceActual] == 'C' || expresion[indiceActual] == 'T')
        estado1(estadoActual, expresion, indiceActual + 1);
    
    else if(expresion[indiceActual] == 'G') 
        estado2(estadoActual, expresion, indiceActual + 1);
}

// Entradas: int* (estado actual), char* (expresión a evaluar), int (numero de caracter a analizar)
// Salidas: ninguna
// Descripción: Representa el estado 2 del autómata finito
void estado2(int* estadoActual, char* expresion, int indiceActual){
    if(indiceActual == MAX_CHAR)
        return;

    *estadoActual = 2;
    
    if(expresion[indiceActual] == 'A' || expresion[indiceActual] == 'C')
        estado1(estadoActual, expresion, indiceActual + 1);
    
    else if(expresion[indiceActual] == 'G')
        estado2(estadoActual, expresion, indiceActual + 1);
    
    else if(expresion[indiceActual] == 'T')
        estado3(estadoActual, expresion, indiceActual + 1);
    
}

// Entradas: int* (estado actual), char* (expresión a evaluar), int (numero de caracter a analizar)
// Salidas: ninguna
// Descripción: Representa el estado 3 del autómata finito
void estado3(int* estadoActual, char* expresion, int indiceActual){
    if(indiceActual == MAX_CHAR) 
        return;

    *estadoActual = 3;
    if(expresion[indiceActual] == 'A')
        estado1(estadoActual, expresion, indiceActual + 1);
    
    else if(expresion[indiceActual] == 'G')
        estado2(estadoActual, expresion, indiceActual + 1);
    
    else if(expresion[indiceActual] == 'T')
        estado3(estadoActual, expresion, indiceActual + 1);
    
    else if(expresion[indiceActual] == 'C')
        estado4(estadoActual, expresion, indiceActual + 1);
}

// Entradas: int* (estado actual), char* (expresión a evaluar), int (numero de caracter a analizar)
// Salidas: ninguna
// Descripción: Representa el estado 4 del autómata finito
void estado4(int* estadoActual, char* expresion, int indiceActual){
    if(indiceActual == 60)
        return;
    
    *estadoActual = 4;
    estado4(estadoActual, expresion, indiceActual + 1);
}
// -----------------------------------------------------------------


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



// Esta función mapea cada linea a cada hebra
int threadByLine(int line){
    return (line / c) % n;
}

void* hebra(void* arg){
    int id = *(int*) arg;
    int i;
    for(i = 0; i < cantidadLineas; i++){
        // Si la linea me corresponde
        if(id == threadByLine(i)){
            // Leer
            pthread_mutex_lock(&entrada_sem);
            int resultado;
            estado1(&resultado, entrada[i],0);
            pthread_mutex_unlock(&entrada_sem);

            // Escribir
            pthread_mutex_lock(&salida_sem);
            salida[i] = resultado;
            pthread_mutex_unlock(&salida_sem);
        }
    }

    pthread_exit((void*) 1);
}