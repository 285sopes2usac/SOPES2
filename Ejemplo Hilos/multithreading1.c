#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

void* myFunction(void* name){

    printf("Imprimiendo desde %s\n", (char*) name);
    sleep(3);
    printf("Trabajo realizado - %s\n", (char*) name);
}

int main() {

    pthread_t thread1, thread2;

    // Creacion de Hilos
    
    pthread_create(&thread1, NULL, myFunction, "Hilo 1");
    pthread_create(&thread2, NULL, myFunction, "Hilo 2");

    // Esperar a los hilos
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("TRABAJO TERMINADO \n");

    return 0;
}