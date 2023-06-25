#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fworker.h"

#define BUFF_SIZE 255
#define print(...) printf("[Worker %s] ", id); printf(__VA_ARGS__); printf("\033[0m\n");

const char* id;
int main(int argc, char const *argv[])
{
    id = argv[1];
    char entrada[BUFF_SIZE];
    int lineasLeidas = 0;
    while(1){
        read(STDIN_FILENO, entrada, BUFF_SIZE);
        if(strcmp(entrada, "FIN") == 0){
            char lineasLeidas_str[BUFF_SIZE];
            sprintf(lineasLeidas_str, "%d", lineasLeidas);
            write(STDOUT_FILENO, lineasLeidas_str, strlen(lineasLeidas_str));
            break;
        }else{
            int resultado;
            estado1(&resultado, entrada, 0);
            lineasLeidas++;
            write(STDOUT_FILENO, resultado == 4 ? "1" : "0",1);
        }
    }
    return 0;
}
