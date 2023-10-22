#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t pid;

    if(argc <= 1){  // En caso de que no haya argumentos
        fprintf(stderr, "Falta el fichero pdf\n");
        exit(EXIT_FAILURE);
    }

    if(access(argv[1], F_OK)){ // Si access devuelve 0 es que todo ha ido bien 
        fprintf(stderr, "El fichero no existe.\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    switch (pid)
    {
    case 0: // Caso en el que sea el hijo
        execlp("evince", "evince", argv[1], NULL); // Hacemos el execlp para que ejecute evince 
        fprintf(stderr, "La ejecución de evince falló.\n");
        exit(EXIT_FAILURE);
        break;
    default: // Caso en el que sea el padre
        sleep(3600);
        wait(NULL); // Espero a que termine mi hijo
        break;
    }
}
