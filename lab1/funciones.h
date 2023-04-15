#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int getCantidadLineas(char* nombreArchivo);
char** leerPrueba(char* nombreArchivo, int* cantidadLineas);
void estado1(int* estadoActual, char* expresion, int indiceActual);
void estado2(int* estadoActual, char* expresion, int indiceActual);
void estado3(int* estadoActual, char* expresion, int indiceActual);
void estado4(int* estadoActual, char* expresion, int indiceActual);