#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

const int MAX_SIZE = 100;

int main(int argc, char const *argv[])
{
    if(argc < 2){
        fprintf(stderr, "Hace falta al menos un fichero pdf\n");
    }

    // Array en el que declaramos si el fichero_i en argv[i] es accesible o no.
    bool accesibles[MAX_SIZE]; 
    
    // Lo inicializamos a true
    for(int i = 0; i<MAX_SIZE; i++) accesibles[i] = true;
    
    // Y lo actualizamos si no está disponible
    for(int i = 1; i<argc; i++){
        if(access(argv[i], F_OK)){ // Si access devuelve 0 es que todo ha ido bien
            fprintf(stderr, "El archivo %s no existe o no es accesible\n", argv[i]); 
            accesibles[i] = 0;
        }
    }

    pid_t pid;

    // Recorremos todos los ficheros accesibles y los abrimos con evince haciendo un fork por fichero
    for(int i = 1; i<argc; i++){
        if(accesibles[i]){
            pid = fork();

            switch (pid)
            {
            case 0: // Caso en que sea el hijo
                execlp("evince", "evince", argv[i], NULL); // Hacemos el execlp para que ejecute evince 
                fprintf(stderr, "La ejecución de evince falló.\n");
                exit(EXIT_FAILURE);
                break;
            default: // Caso en que sea el padre
                wait(NULL);
                break;
            }
        }
    }
}