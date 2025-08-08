// SOLO CREAR SI NO SE USARAN MACROS PARA CREAR LA SYSCALL -> SIN USAR SYSCALL_DEFINEX(...)
#ifndef _SYSCALL_USAC_H
#define _SYSCALL_USAC_H

#include <linux/types.h> // Para usar tipo long del kernel
#include <linux/syscalls.h> // Para usar utilidades de llamadas al sistema

asmlinkage long __x64_sys_usac_hello(void); // Declaración de la syscall

// asm es por ensamblador, y asmlinkage indica como se pasaran los argumentos
// Normalmente es atraves de registros, pero con asmlinkage es a través de la pila
// sys_usac_hello es el nombre de la funcion de la syscall y se debe agregar el prefijo __ARCH_
// mismo nombre que se le dará en syscall_usac.c
// long es el tipo de dato que retorna la syscall

#endif
