#include "funciones.h"

// Obtiene la cantidad de lineas del nombre del archivo después del separador "_"
// Ej: prueba_10.txt -> 10, prueba_100.txt = 100
int getCantidadLineas(char* nombreArchivo){
    char* posicionSeparador = strchr(nombreArchivo, '_') + 1;
    return atoi(posicionSeparador);
}

// Entrada: nombre del archivo a leer con extensión
// Salida: Matriz de cadena de caracteres del contenido del archivo
// Lee un archivo de principio a fin
char** leerPrueba(char* nombreArchivo, int* cantidadLineas){
    int lenLinea = 60;

    FILE* archivo = fopen(nombreArchivo, "r");
    if(archivo == NULL){
        printf("No se ha podido leer el archivo\n");
        exit(1);
    }

    *cantidadLineas = getCantidadLineas(nombreArchivo);
    // printf("Leyendo: %s    Lineas: %d\n", nombreArchivo, *cantidadLineas);

    // Leer
    char** salida = (char**) malloc(sizeof(char*) * *cantidadLineas);
    for(int i = 0; i < *cantidadLineas; i++){
        salida[i] = (char*) malloc(sizeof(char) * lenLinea);
        fscanf(archivo, "%s", salida[i]);
    }

    fclose(archivo);
    return salida;
}

// --------------------- ESTADOS ------------------------ //
void estado1(int* estadoActual, char* expresion, int indiceActual){
    if(indiceActual == 59){
        return;
    }
    *estadoActual = 1;
    if(expresion[indiceActual] == 'A' || expresion[indiceActual] == 'C' || expresion[indiceActual] == 'T'){
        estado1(estadoActual, expresion, indiceActual + 1);
    }else if(expresion[indiceActual] == 'G'){
        estado2(estadoActual, expresion, indiceActual + 1);
    }
}
void estado2(int* estadoActual, char* expresion, int indiceActual){
    if(indiceActual == 59){
        return;
    }
    *estadoActual = 2;
    if(expresion[indiceActual] == 'A' || expresion[indiceActual] == 'C' || expresion[indiceActual] == 'G'){
        estado1(estadoActual, expresion, indiceActual + 1);
    }else if(expresion[indiceActual] == 'T'){
        estado3(estadoActual, expresion, indiceActual + 1);
    }
}
void estado3(int* estadoActual, char* expresion, int indiceActual){
    if(indiceActual == 59){
        return;
    }
    *estadoActual = 3;
    if(expresion[indiceActual] == 'A' || expresion[indiceActual] == 'G'){
        estado1(estadoActual, expresion, indiceActual + 1);
    }else if(expresion[indiceActual] == 'C'){
        estado4(estadoActual, expresion, indiceActual + 1);
    }else if(expresion[indiceActual] == 'T'){
        estado3(estadoActual, expresion, indiceActual + 1);
    }
}

void estado4(int* estadoActual, char* expresion, int indiceActual){
    if(indiceActual == 59){
        return;
    }
    *estadoActual = 4;
    estado4(estadoActual, expresion, indiceActual + 1);
}
// -----------------------------------------------------------------