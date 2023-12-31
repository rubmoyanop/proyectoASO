#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>

void punteros(void)
{
    int a[4];
    int *b = malloc(16); // Reserva 16 bytes del heap y deja en b la dirección de memoria del comienzo del bloque reservado
    int *c = NULL;
    int i;

    printf("1: a = %p, b = %p, c = %p\n", a, b, c);

    c = a;
    for (i = 0; i < 4; i++)
        a[i] = 100 + i;
    c[0] = 200;
    printf("2: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n", a[0], a[1], a[2], a[3]);

    c[1] = 300;
    *(c + 2) = 301;
    printf("3: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n", a[0], a[1], a[2], a[3]);

    c = c + 1;
    *c = 400;
    printf("4: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n", a[0], a[1], a[2], a[3]);

    c = (int *)((char *)c + 1);
    *c = 500;
    printf("5: a[0] = %d, a[1] = %d, a[2] = %d, a[3] = %d\n", a[0], a[1], a[2], a[3]);

    b = (int *)a + 1;
    c = (int *)((char *)a + 1);
    printf("6: a = %p, b = %p, c = %p\n", a, b, c);
}

int main(int argc, char **argv)
{
    punteros();

    return EXIT_SUCCESS;
}
