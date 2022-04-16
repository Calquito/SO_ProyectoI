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
#include <string.h>

#define SHM_SEMS "sems_shared_memory" // Nombre de la memoria compartida


typedef struct {
    char character[1];
    char date[40];
    char hour[40];
}item;

typedef struct{
    item  shared_var[100];
    sem_t sem1;
} Sems;




int main(){

    int fd_shm = shm_open(SHM_SEMS, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

    ftruncate(fd_shm, sizeof(Sems));

    Sems *sems = mmap(NULL, sizeof(Sems), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);

    sem_init(&sems->sem1, 1, 1);

    sem_wait(&sems->sem1);
    strcpy((sems->shared_var + 1)->character, "a");
    strcpy((sems->shared_var + 1)->date, "Date");
    strcpy((sems->shared_var + 1)->hour, "Hour");
    sem_post(&sems->sem1);



    int salir = 1;
    while(salir == 1){        

        for(int a = 0; a < 100; a++){
            sem_wait(&sems->sem1);
            printf("%p : %c, %s, %s\n", &(*(sems->shared_var + a)), *(sems->shared_var + a)->character, (sems->shared_var + a)->date, (sems->shared_var + a)->hour);
            sem_post(&sems->sem1);
        }        
       
        printf("Actualizar? Si -> 1    No(Terminar) -> 0 \n");
        scanf(" %d",&salir);
        
    }

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
