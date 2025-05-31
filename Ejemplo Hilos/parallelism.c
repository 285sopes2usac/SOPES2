#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>  // Para la función sleep
#include <sys/time.h>  // Para medir el tiempo de ejecución

#define NUM_THREADS 4

void timeDiff(struct timeval start, struct timeval end, long* seconds, long *micros){

    *seconds = end.tv_sec - start.tv_sec; // segundos
    *micros = end.tv_usec - start.tv_usec; // microsegundos
    if (micros < 0) {
        *micros += 1000000; // sumamos 1 segundo a los micros para compensar el negativo
        *seconds--; // restamos 1 segundo
    }
}

// Función que será ejecutada por cada hilo
void* tarea(void* arg) {
    // Obtener el ID del hilo
    int thread_id = *((int*)arg);
    
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Simulamos una tarea que toma tiempo
    printf("Hilo %d comenzando su tarea...\n", thread_id);
    sleep(5);  // Simula trabajo (2 segundos)

    gettimeofday(&end, NULL);

    long seconds, micros;
    timeDiff(start, end, &seconds, &micros);

    printf("Hilo %d terminó su tarea. Tiempo transcurrido: %ld segundos y %ld microsegundos.\n", thread_id, seconds, micros);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    struct timeval start, end;
    gettimeofday(&start, NULL); // variable para guardar valor y timezone

    // Crear múltiples hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        int *mspace = malloc(sizeof(int));
        *mspace = i;
        pthread_create(&threads[i], NULL, tarea, (void*)mspace);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Medir el tiempo total
    gettimeofday(&end, NULL);

    long seconds, micros;
    timeDiff(start, end, &seconds, &micros);

    printf("Tiempo total de ejecución del programa: %ld segundos y %ld microsegundos.\n", seconds, micros);

    return 0;
}