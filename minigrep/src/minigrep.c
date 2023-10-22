#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>

#define MAX_BUFFER_SIZE 1048576 // (1 MB = 1024^2 Bytes)
#define MIN_BUFFER_SIZE 1
#define MAX_LINE_SIZE 4096

int coincideExpresionRegular(char *cadena, regex_t *regex){
    regmatch_t pmatch[2];
    return !regexec(regex, cadena, 2, pmatch, 0);
}

void leerLinea(char *buffer, char* linea, int *indiceLectura)
{
    int i = 0;
    while (buffer[*indiceLectura] != '\n' && *indiceLectura <= MAX_LINE_SIZE)
    {
        linea[i] = buffer[*indiceLectura];
        (*indiceLectura)++;
        i++;
    }

    if (*indiceLectura <= MAX_LINE_SIZE)
    {
        (*indiceLectura)++;
        linea[i] = '\0';
    }
    else{
        // Ver que hacemos en los casos en los que el buffer de lectura no acabe en \n por ejemplo: ["hola\nadios\nhastalu"]
    }
}

void llenarBufferLectura(char *buffer, int bufferSize)
{
    ssize_t bytesRead = 0;
    ssize_t totalBytesRead = 0;

    while (totalBytesRead < bufferSize)
    {
        bytesRead = read(STDIN_FILENO, buffer + totalBytesRead, bufferSize - totalBytesRead);

        if (bytesRead == -1)
        {
            perror("Error al leer de la entrada estándar");
            exit(EXIT_FAILURE);
        }
        else if (bytesRead == 0)
        {
            break; // Pues no hay más que leer de la entrada estándar
        }

        totalBytesRead += bytesRead;
    }
}

int main(int argc, char **argv){
    int opt, flag = 0, bufferSize = 0;
    char *regex_patron = NULL;

    optind = 1;

    while ((opt = getopt(argc, argv, "r:s:vh")) != -1) 
    {
        switch (opt)
        {
        case 'r':
            regex_patron = optarg;
            break;
        case 's':
            bufferSize = atoi(optarg);
            break;
        case 'v':
            flag = 1;
            break;
        case 'h':
        default:
            fprintf(stderr, "Uso: %s -r REGEX [-s BUFSIZE] [-v] [-h]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if(regex_patron==NULL){
        fprintf(stderr, "Uso: %s -r REGEX [-s BUFSIZE] [-v] [-h]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (bufferSize > MAX_BUFFER_SIZE)
    {
        fprintf(stderr, "El tamaño máximo para el buffer es de %d bytes.\n", MAX_BUFFER_SIZE);
        exit(EXIT_FAILURE);
    }

    // printf("r: \"%s\", s: %d, v: %d\n", regex_patron, bufferSize, flag);

    regex_t regex;

    if(-1==(regcomp(&regex, regex_patron, REG_EXTENDED | REG_NEWLINE))){
        perror("Error al compilar la expresión regular.");
        exit(EXIT_FAILURE);
    }

    char *bufferLectura = (char *)malloc(bufferSize);
    if(bufferLectura == NULL){
        perror("malloc buffer lectura");
        exit(EXIT_FAILURE);
    }

	char *bufferEscritura = (char *)malloc(bufferSize);
    if(bufferEscritura == NULL){
        perror("malloc buffer escritura");
        exit(EXIT_FAILURE);
    }
    
    char *intermedio = (char *)malloc(MAX_LINE_SIZE * sizeof(char));
    if(intermedio == NULL){
        perror("malloc buffer intermedio");
        exit(EXIT_FAILURE);
    }

    llenarBufferLectura(bufferLectura, bufferSize);

    int indiceLectura = 0;
    leerLinea(bufferLectura, intermedio, &indiceLectura);

    if(coincideExpresionRegular(intermedio, &regex)){
        printf("Coincide!\n");
    }

    llenarBufferLectura(bufferLectura, bufferSize);
    leerLinea(bufferLectura, intermedio, &indiceLectura);






    free(bufferEscritura);
    free(bufferLectura);
    free(intermedio);
}