#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock;

void* thread(void* args){
    char* name = (char*) args;

    
    pthread_mutex_lock(&lock);
    
    // Simular Zona Critica
    printf("Inicio - %s\n", name);
    sleep(4);
    printf("Termina - %s\n", name);

    // Signal
    pthread_mutex_unlock(&lock);
    
    return NULL;
}

int main () {

    pthread_mutex_init(&lock, NULL); // Inicializar Mutex

    pthread_t t1,t2;

    pthread_create(&t1, NULL, thread, "Hilo 1");
    pthread_create(&t2, NULL, thread, "Hilo 2");

    // Esperar a los hilos

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}