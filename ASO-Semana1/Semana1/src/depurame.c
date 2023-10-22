#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int TAMARRAY = 20000;

// Crea un array de punteros a carácter
char **inicializa()
{
    char **tmp;
    int i;

    tmp = malloc(TAMARRAY * sizeof(char *));
    for (i = 0; i < TAMARRAY; i++)
        tmp[i] = NULL;

    return tmp;
}

void copia(char **buffers)
{
    buffers[0] = malloc(strlen("ASO")*sizeof(char));
    strcpy(buffers[0], "ASO");
}

int main(void)
{
    char **buffers;

    buffers = inicializa();
    copia(buffers);
    printf("%s\n", buffers[0]);

    for(int i = 0; i<TAMARRAY; i++){
        if(buffers[i] != NULL)
            free(buffers[i]);
    }
    free(buffers);
    return EXIT_SUCCESS;
}