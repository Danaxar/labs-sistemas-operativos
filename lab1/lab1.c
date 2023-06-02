#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "funciones.h"
#define false 0
#define true 1
#define boolean int

/*
Entradas:
    - argc: Cantidad de argumentos + 1 (considera el nombre del ejecutable)
    - argv: Lista de argumentos
Salidas: Entero
*/
int main(int argc, char *argv[]){
    // Tienen que ser 5 o 6 argumentos
    if(!(argc == 6 || argc == 5)){
        printf("Argumentos invalidos.\n");
        exit(0);
    }

    // Getopt
    int option;
    char* i = NULL;
    char* o = NULL;
    boolean b = false;
    while((option = getopt(argc, argv, "i:o:b")) != -1){
        switch (option){
        case 'i':
            i = (char*) malloc(strlen(optarg) + 1);
            strcpy(i, optarg);
            break;
        case 'o':
            o = (char*) malloc(strlen(optarg) + 1);
            strcpy(o, optarg);
            break;
        case 'b':
            b = true;
            break;
        }
    }

    // Crear stream para archivo de salida
    FILE* salida = fopen(o, "w");

    // Procesamiento
    int cantidadLineas;
    char** contenido = leerPrueba(i, &cantidadLineas);  // Leer archivo de entrada
    int cantRegulares = 0;
    int cantNoRegulares = 0;
    for(int i = 0; i < cantidadLineas; i++){
        // Mostrar expresión
        fprintf(salida, "%s", contenido[i]);
        b ? printf("%s  ", contenido[i]) : 0;
        
        // Ejecutar reconocedor
        int estado = 1;
        estado1(&estado, contenido[i], 0);

        // Verificar si cumple
        if(estado == 4){
            fprintf(salida, "si\n");
            cantRegulares++;
            b ? printf("si\n") : 0;
        }else{
            fprintf(salida, "no\n");
            cantNoRegulares++;
            b ? printf("no\n") : 0;
        }
    }

    // Salida
    printf("\n\n");
    if(b){
        printf("Total de expresiones que Si son regulares: %d\n", cantRegulares);
        printf("Total de expresiones que No son regulares: %d\n", cantNoRegulares);
        printf("Total de lineas leídas: %d\n", cantidadLineas);
    }
    fprintf(salida, "\n\n");
    fprintf(salida, "Total de expresiones que Si son regulares: %d\n", cantRegulares);
    fprintf(salida, "Total de expresiones que No son regulares: %d\n", cantNoRegulares);
    fprintf(salida, "Total de lineas leídas: %d\n", cantidadLineas);

    // Liberar memoria
    fclose(salida);
    free(i);
    free(o);
    for(int i = 0; i < cantidadLineas; i++) free(contenido[i]);
    free(contenido);
    return 0;
}