#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    char* holamundo = "Hola mundo\n";
    write(STDOUT_FILENO, holamundo, strlen(holamundo));
    return 0;
}
