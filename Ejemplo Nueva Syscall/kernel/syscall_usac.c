#include <linux/syscalls.h> // Para usar macros de llamadas al sistema
// #include <linux/syscall_usac.h> // Incluir el header de la syscall si se usa la creacion manual, sin macros

// SYSCALL_DEFINE0 es una macro que define una syscall sin argumentos
// El 0 indica que no se esperan argumentos para esta syscall
// Se puede usar desde el 0 hasta el 6 para definir syscalls con hasta 6 argumentos
// Este limite de 6 argumentos es por el uso de registros en la arquitectura x86_64

// sys_usac_hello es el nombre de la syscall que se va a implementar
SYSCALL_DEFINE0(usac_hello) // Definición de syscall sin argumentos
{
    printk(KERN_INFO "USAC Syscall ejecutada\n");
    // Alternativa usar macro para log nivel info: // pr_info("Hello from usac syscall!\n");
    return 42;
}

// Forma alterna de crear la implementacion sin usar macros
// El nombre de la funcion debe ser igual a la definida en la cuarta columna
// de la tabla de syscalls agregando el prefijo __ARCH_, ej: __x64_
// asmlinkage long __x64_sys_usac_hello(void){
//     printk(KERN_INFO "USAC Syscall ejecutada\n");
//     return 42;
// }
