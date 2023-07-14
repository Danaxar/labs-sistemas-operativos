// Daniel Catalán 20.675.871-6
// Felipe Carrasco 20.285.756-6

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CHAR 60
void estado1(int* estadoActual, char* expresion, int indiceActual);
void estado2(int* estadoActual, char* expresion, int indiceActual);
void estado3(int* estadoActual, char* expresion, int indiceActual);
void estado4(int* estadoActual, char* expresion, int indiceActual);




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


// Monitor
typedef struct monitor_archivo_t{
    char** lineas;
    int* resultados;
    int len;
    pthread_mutex_t fileSem;
    pthread_mutex_t resultSem;
}monitor_archivo;

monitor_archivo* monitor;

void iniciarMonitorArchivo(monitor_archivo * monitorArchivo, char* nombreArchivo){
    monitorArchivo = (monitor_archivo*) malloc(sizeof(monitor_archivo));
    monitorArchivo->lineas = leerArchivo(nombreArchivo, &monitorArchivo->len);
    monitorArchivo->resultados = (int*) malloc(sizeof(int) * monitorArchivo->len);
    pthread_mutex_init(&monitorArchivo->fileSem, NULL);
    pthread_mutex_init(&monitorArchivo->resultSem, NULL);
}

// Leer
char* getLinea(monitor_archivo * monitorArchivo, int index){
    pthread_mutex_lock(&monitorArchivo->fileSem);
    char* salida = monitorArchivo->lineas[index];
    pthread_mutex_unlock(&monitorArchivo->fileSem);
    return salida;
}

// Escribir
void setResultado(monitor_archivo * monitorArchivo, int index, int resultado){
    pthread_mutex_lock(&monitorArchivo->resultSem);
    monitorArchivo->resultados[index] = resultado;
    pthread_mutex_unlock(&monitorArchivo->resultSem);
    return;
}

int c; // Tamaño del chunk, cantidad de lineas a leer por chunk
int n; // Cantidad de hebras a generar

// Esta función mapea cada linea a cada hebra
int threadByLine(int line){
    return (line / c) % n;
}

void* hebra(void* id){
    int idHebra = (int)(long) id;
    // Recorrer todas las lineas
    int n = monitor->len;
    for(int i = 0; i < n; i++){
        // Si la linea me corresponde
        if(idHebra == threadByLine(i)){
            int resultado;
            char* linea = getLinea(monitor, i);
            estado1(&resultado, linea,0);
            setResultado(monitor, i, resultado);
        }
    }
}

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

    iniciarMonitorArchivo(monitor, i);

    // Crear hebras
    pthread_t* idsHebras = (pthread_t*) malloc(sizeof(pthread_t) * n);
    for(int i = 0; i < n; i++){
        pthread_create(&idsHebras[i], NULL, hebra, (void*) &idsHebras[i]);
    }

    // Esperar a que las hebras terminen
    for(int i = 0; i < n; i++){
        pthread_join(idsHebras[i], NULL);
    }

    // Mostrar por pantalla
    for(int i = 0; i < monitor->len; i++){
        printf("%s  %s\n", monitor->lineas[i], monitor->resultados[i] ? "si" : "no");
    }

    pthread_exit(0);

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