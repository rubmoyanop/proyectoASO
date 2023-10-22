#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 4096

/*  Ejemplo: cat_mem_din [-o FILEOUT] [FILEIN1 FILEIN2 ... FILEINn]
    Notas:   Por defecto, se escribe en 'stdout' y se lee de 'stdin'.
*/

void print_help(char* program_name)
{
    fprintf(stderr, "Uso: %s [-o FILEOUT] [FILEIN1 FILEIN2 ... FILEINn]\n", program_name);
}

void catfd(int fdin, int fdout, char *buf, unsigned buf_size)
{
    ssize_t num_read, num_written;

    ssize_t aux_written = 0; // Variable auxiliar para tratar Las Escriturars Parciales 

    while ((num_read = read(fdin, buf, buf_size)) > 0)
    {   
        aux_written = 0;
        while((num_written = write(fdout, buf+aux_written, num_read-aux_written)) > 0){
            aux_written += num_written;
        }
        if (num_written == -1)
        {
            perror("write(fdin)");
            exit(EXIT_FAILURE);
        }
    }

    if (num_read == -1)
    {
        perror("read(fdin)");
        exit(EXIT_FAILURE);
    }
}

void exit_with_error(char *motivo){
    perror(motivo);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int opt;
    char *fileout = NULL;
    int fdout;
    int fdin;
    char *buf;
    int fderr; // Para el último check

    optind = 1;
    while ((opt = getopt(argc, argv, "o:h")) != -1)
    {
        switch (opt)
        {
        case 'o':
            fileout = optarg;
            break;
        case 'h':
            print_help(argv[0]);
            exit(EXIT_SUCCESS);
        default:
            print_help(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    /* Abre el fichero de salida */
    if (fileout != NULL)
    {
        fdout = open(fileout, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fdout == -1)
        {
            perror("open(fileout)");
            exit(EXIT_FAILURE);
        }
    }
    else /* Por defecto, la salida estándar */
        fdout = STDOUT_FILENO;

    /* Reserva memoria dinámica para buffer de lectura */
    if ((buf = (char *) malloc(BUF_SIZE * sizeof(char))) == NULL)
    {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    // Para redirigir la salida estándar de error a un fichero llamado error.log con dup2().
    fderr = open("error.log", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if(fderr==-1){
        exit_with_error(fderr);
    }

    if(dup2(fderr, STDERR_FILENO) == -1){
        exit_with_error(fderr);
    }
    
    close(fderr);

    /* Abre cada fichero de entrada y lo escribe en 'fileout' */
    if (optind < argc)
        for (int i = optind; i < argc; i++)
        {
            int fdin = open(argv[i], O_RDONLY);
            if (fdin == -1)
            {
                perror("open(filein)");
                continue;
            }
            catfd(fdin, fdout, buf, BUF_SIZE);
            if (close(fdin) == -1)
            {
                perror("close(fdin)");
                exit(EXIT_FAILURE);
            }
        }
    else
    {
        catfd(STDIN_FILENO, fdout, buf, BUF_SIZE);
    }

    /* Cierra el fichero de salida */
    if (fileout != NULL)
    {
        if (close(fdout) == -1)
        {
            perror("close(fdout)");
            exit(EXIT_FAILURE);
        }
    }

    /* Libera memoria dinámica de buffer de lectura */
    free(buf);

    exit(EXIT_SUCCESS);
}
