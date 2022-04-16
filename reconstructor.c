/*
   It reads from a previously created memory object "/myMemoryObj"
   to be compiled with "-lrt"
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

#define SHM_SEMS "sems_shared_memory" // Nombre de la memoria compartida

int main(void)
{
    typedef struct
    {
        char character[1];
        char date[40];
        char hour[40];
    } item;

    typedef struct
    {
        item shared_var[100];
        sem_t sem1;
    } Sems;

    int fd;
    char *ptr;
    struct stat shmobj_st;

    int fd_shm = shm_open(SHM_SEMS, O_RDWR, 0666);
    if (fd_shm == -1)
    { // si el buffer no está inicializado.
        puts("Buffer no existe");
        return 0;
    }
    Sems *sems = mmap(NULL, sizeof(Sems), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Map failed in read process: %s\n", strerror(errno));
        exit(1);
    }

    int i = 0;
    while (1)
    {
        // wait for enter key pressed
        char key_pressed = getchar();
        if (key_pressed == '\n')
        {

            FILE *fichero;
            fichero = fopen("TextoReconstruido.txt", "a");
            sem_wait(&sems->sem1);
            char file_char = *(sems->shared_var + i)->character;
            printf("Caracter: ");
            putchar(file_char);
            printf("\nDireccion: %p", &(*(sems->shared_var + i)->character));
            printf("\nFecha: %s", (sems->shared_var + i)->date);
            printf("\nHora: %s", (sems->shared_var + i)->hour);
            fputc(file_char, fichero);
            sem_post(&sems->sem1);
            printf("\n");
            i++;
            fclose(fichero);

            FILE *fR = fopen("TextoReconstruido.txt", "r");

            if (fR == NULL)
            {
                printf("Error: no se pudo encontrar el archivo");
                printf("\n");
                return 1;
            }

            // read one character at a time and
            // display it to the output
            printf("\n El contenido reconstruido es: ");
            char entire_file_char;
            while ((entire_file_char = fgetc(fR)) != EOF)
            {

                putchar(entire_file_char);
            }
            printf("\n");
            fclose(fR);
        }
    }

    close(fd);

    return 0;
}
