#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

struct args {
    int numero;
    char* cadena;
};

void* myFunction(void* input){
    printf("Hilo iniciado\n");

    struct args p = *((struct args*)input);

    printf("Cadena: %s\n", p.cadena);
    printf("Numero: %d\n", p.numero);
    sleep(5);

    printf("HILO TERMINADO\n");
}

int main(){
    pthread_t thread;
    int err;

    struct args myArgs;
    myArgs.cadena = "Hola Mundo";
    myArgs.numero = 12;

    // pthread_create devuelve 0 si tuvo exito o un numero mayor a 0 indicando el codigo de error
    err = pthread_create(&thread, NULL, myFunction, (void*) &myArgs);
    
    // 0 = false, >0 = true
    if(err){
        printf("Error al crear el hilo: %d\n", err);
        return 1;
    }

    pthread_join(thread, NULL);
    printf("TRABAJO TERMINADO");

    return 0;
}