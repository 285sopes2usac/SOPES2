#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

#include<semaphore.h>
  
sem_t s; 
  
void* thread(void* arg) 
{ 
    //wait 
    sem_wait(&s); 
    printf("Inicio - %s\n", (char*) arg); 
  
    sleep(3); 
      
    //signal 
    printf("Termino - %s\n", (char*) arg); 
    sem_post(&s); 

    return NULL;
} 
  
int main() 
{ 
    sem_init(&s, 0, 3);  // Inicializamos nuestro semaforo

    pthread_t threads[4];

    for(int i = 0; i < 4; i++){
        char *thread_name = (char*) malloc(12*sizeof(char));
        sprintf(thread_name, "Hilo %d", i);
        pthread_create(&threads[i], NULL, thread, thread_name);
    }

    for(int i = 0; i < 4; i++){
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&s);  // Liberamos los recursos del semaforo

    return 0; 
} 