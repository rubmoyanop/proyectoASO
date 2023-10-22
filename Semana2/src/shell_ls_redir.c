#define _POSIX_C_SOURCE 200809L
// Son los includes necesarios para que vayan ciertas funciones. Por ejemplo, si quieremos usar
// la función close(), hacemos en la terminal -> man 2 close y nos aparecerá en SYNOPSIS
// la librería que tenemos que incluir
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    pid_t pid; /* Usado en el proceso padre para guardar el PID del proceso hijo */
    int fd;
    char *argumentos[]={"ls", "-la", NULL};

    switch (pid = fork())
    {
    case -1: /* fork() falló */
        perror("fork()");
        exit(EXIT_FAILURE);
        break;
    case 0:                             /* Ejecución del proceso hijo tras fork() con éxito */
        if (close(STDOUT_FILENO) == -1) /* Cierra la salida estándar */
        {
            perror("close()");
            exit(EXIT_FAILURE);
        }
        /* Abre el fichero "listado" al que se asigna el descriptor de fichero no usado más bajo, es decir, STDOUT_FILENO(1) */
        if ((fd = open("listado", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP)) == -1) // WRite ONLY; Si no está el fichero crealo; Si está creado, lo limpias; Quiero que tenga (r)ead (w)rite e(x)ecute para el usuario. Para ver el valor de las flags hacemos man 2 open
        {
            // Si me da error abrir el fichero
            perror("open()");
            exit(EXIT_FAILURE);
        }
        // Si no me da error el cerrar ni el abrir
        execvp("ls", argumentos); /* Sustituye el binario actual por /bin/ls */ // El último tiene que ser NULL para indicar que no hay más parámetros
        fprintf(stderr, "execlp() failed\n"); /* Esta línea no se debería ejecutar si la anterior tuvo éxito, pues el exec borra toda la memoria */
        exit(EXIT_FAILURE);
        break;
    default:                  /* Ejecución del proceso padre tras fork() con éxito */
        if (wait(NULL) == -1) /* Espera a que termine el proceso hijo */ // Que se use wait(NULL) significa que no me importa lo que me devuelva el hijo, sime importara aetndría que pasarle un puntero a entero
        {
            perror("wait()");
            exit(EXIT_FAILURE);
        }
        break;
    }

    return EXIT_SUCCESS;
}
