// gcc -o creador creador.c -pthread -lrt
// ./creador

#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHM_SEMS "sems_shared_memory" // Nombre de la memoria compartida

typedef struct{
    char shared_var[100];
} Sems;

int main(){

    int fd_shm = shm_open(SHM_SEMS, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

    ftruncate(fd_shm, sizeof(Sems));

    Sems *sems = mmap(NULL, sizeof(Sems), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);

    int salir = 1;
    int pos = 0;
    while(salir == 1){

        while(pos < 10){
            printf("Direccion de memoria: %p \n", &sems->shared_var[pos]);
            printf("Caracter: %c \n", sems->shared_var[pos]);
            pos++;
        }
        pos = 0;

        printf("Salir \n");
        scanf(" %d",&salir);
        
    }

    int terminar;
    printf("Terminar programa \n");
    scanf(" %d",&terminar);

    // Destruir memoria
    munmap(sems, sizeof(Sems));

    shm_unlink(SHM_SEMS);

    return 0;
}

/*
cd /dev
cd shm
rm sems_shared_memory_1
*/
