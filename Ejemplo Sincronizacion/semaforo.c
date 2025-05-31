#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

#define NUM_THREADS 4
#define INIT_RESOURCES 2

typedef struct {
    int recursos;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
} semaforo;

semaforo sem;


void semaforo_init(semaforo* sem, int initial_value){
    pthread_mutex_init(&(sem->mutex), NULL); // inicializo el mutex
    pthread_cond_init(&(sem->condition), NULL); // inicializo la condicion
    sem->recursos = initial_value; // asigno una cantidad inicial de recursos
}

void semaforo_wait(semaforo* sem){

    pthread_mutex_lock(&(sem->mutex)); // bloqueo el mutex para poder leer el valor actual de recursos

    while(sem->recursos <= 0){ // si no hay recursos, espero a que la condicion cambie
        pthread_cond_wait(&(sem->condition), &(sem->mutex));
    }

    sem->recursos--;
    pthread_mutex_unlock(&(sem->mutex));
}

void semaforo_signal(semaforo* sem){
    
    pthread_mutex_lock(&(sem->mutex));
    sem->recursos++;
    pthread_cond_signal(&(sem->condition)); // Enviar señal para despertar hilos esperando condicion
    pthread_mutex_unlock(&(sem->mutex)); // Liberar el mutex
}

void* thread(void* args){
    char* name = (char*) args;

    printf("%s en espera...\n", name);
    semaforo_wait(&sem);

    printf("%s obtuvo recursos\n", name);
    sleep(3);
    printf("%s liberando recursos\n", name);

    semaforo_signal(&sem);
}

int main(){

    semaforo_init(&sem, INIT_RESOURCES);
    pthread_t threads[NUM_THREADS];

    for(int i=0;i<NUM_THREADS;i++){
        char* name = (char*) malloc(12*sizeof(char));
        sprintf(name, "Hilo %d", i);
        pthread_create(&threads[i], NULL, thread, (void*)name);
    }

    for(int i=0;i<NUM_THREADS;i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}
