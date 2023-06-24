#include "fworker.h"
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