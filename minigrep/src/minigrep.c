#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>

int coincideExpresionRegular(char *cadena, regex_t *regex){
    regmatch_t pmatch[2];
    return !regexec(regex, cadena, 2, pmatch, 0);
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

    printf("r: \"%s\", s: %d, v: %d\n", regex_patron, bufferSize, flag);

    regex_t regex;

    if(-1==(regcomp(&regex, regex_patron, REG_EXTENDED | REG_NEWLINE))){
        perror("regcomp");
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
}