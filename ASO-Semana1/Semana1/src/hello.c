#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h> // En todos los programas va a haber que añadir stdlib.h (Por ejemplo incluye EXIT_SUCCESS)
#include <sys/types.h> // Para getpid(), hemos visto que hace falta usando el man
#include <unistd.h> // Para getpid()


int main(void)
{
    printf("Hello, ASO! y m ipid es: %d\n", getpid());
    return EXIT_SUCCESS;
}