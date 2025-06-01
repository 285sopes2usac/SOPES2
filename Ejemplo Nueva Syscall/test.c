#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

#define SYSCALL_USAC 548  // Asegúrate de poner el número correcto

int main() {
    long res = syscall(SYSCALL_USAC);
    printf("Resultado: %ld\n", res);
    return 0;
}