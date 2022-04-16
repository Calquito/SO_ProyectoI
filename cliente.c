// gcc -o cliente cliente.c -pthread -lrt
// ./cliente

#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define SHM_SEMS "sems_shared_memory" // Nombre de la memoria compartida

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

int main()
{

    int fd_shm = shm_open(SHM_SEMS, O_RDWR, 0666);
    if (fd_shm == -1)
    { // si el buffer no está inicializado.
        puts("Buffer no existe");
        return 0;
    }
    Sems *sems = mmap(NULL, sizeof(Sems), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);

    /*int cont = 0;
    int a = 1;

    while (a == 1)
    {


        printf("Pulse la tecla enter para continuar\n");
        getchar();



        sems->shared_var[cont] = 'a';

        cont ++;
    }*/

    // global variable requested?
    bool finished = false;

    // Get file specified by user
    char filename[40];

    // pointer to the begining of the shared memory
    char char_at_first_position = 'a';
    char *pointer_to_first_position = &char_at_first_position;

    printf("Ingrese el nombre del archivo a cargar: \n");
    scanf("%s", filename);

    // opens file, and reads character by character
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: no se pudo encontrar el archivo %s", filename);
        printf("\n");
        return 1;
    }

    // read one character at a time and
    // display it to the output
    printf("El contenido del archivo a cargar es: \n");
    char entire_file_char;
    while ((entire_file_char = fgetc(fp)) != EOF)
    {
        putchar(entire_file_char);
    }
    printf("\n");

    // user enters character when presses enter
    // opens the file again to read character by character
    fp = fopen(filename, "r");
    // gets the chars from the file
    char file_char;

    // number of the pointer
    int pointer_cont = 0;

    // current direction of pointer
    int i = 0;

    while ((file_char = fgetc(fp)) != EOF)
    {
        // wait for enter key pressed
        printf("Presione enter para continuar...\n");
        getchar();
        //*(pointer_to_first_position + pointer_cont) = file_char;

        sem_wait(&sems->sem1);
        *(sems->shared_var + i)->character = file_char;
        strcpy((sems->shared_var + i)->date, "Date");
        strcpy((sems->shared_var + i)->hour, "Hour");

        printf("Direccion: %p \n", &(sems->shared_var + i)->character);
        sem_post(&sems->sem1);

        i++;

        // putchar(file_char);

        // printf("Direccion: %p", &sems->shared_var[i]);
        pointer_cont++;
    }

    // file finished to be readed
    finished = true;

    // close the file
    fclose(fp);

    return 0;
}