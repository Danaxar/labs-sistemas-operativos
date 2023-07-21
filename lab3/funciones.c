#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "funciones.h"
#define MAX_CHAR 60
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