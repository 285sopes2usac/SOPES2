#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

void* myFunction(void* id){

    int *thread_id = (int*)id;
    printf("Imprimiendo desde hilo %d\n", *thread_id);
    sleep(3);
    printf("Trabajo realizado - Hilo %d\n", *thread_id);
    return id;
}

int main(){

    pthread_t thread[5];

    for(int i= 0; i<5; i++){
        int *mspace = malloc(sizeof(int));
        *mspace = i;

        //Creacion de Hilo
        pthread_create(&thread[i], NULL, myFunction, (void*) mspace);
    }

    //Esperar hilos

    for(int i= 0; i<5; i++){
        pthread_join(thread[i], NULL);
    }

    printf("TRABAJO TERMINADO\n");

    return 0;
}


