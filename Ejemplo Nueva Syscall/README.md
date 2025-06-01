# Instrucciones para compilar y probar la nueva syscall

## 1. Compilar el kernel con la nueva syscall

1. Guarda todos los cambios en los archivos modificados (`syscall_64.tbl`, `syscall_usac.c`, `syscall_usac.h`).
2. Desde la raíz del código fuente del kernel, ejecuta:
   ```bash
   fakeroot make -j$(nproc)
   ```

3. Instala el nuevo kernel (ajusta la ruta según tu distribución):
   ```bash
   sudo make modules_install
   sudo make install
   ```

4. Reinicia y selecciona el nuevo kernel en el menú de arranque:
   ```bash
   sudo reboot
   ```
5. Verifica que estás usando el kernel correcto:
   ```bash
   uname -r
   ```
   Debe coincidir con la versión que compilaste.

---

## 2. Compilar y ejecutar el programa de prueba

1. Compila `test.c`:
   ```bash
   gcc test.c -o test
   ```
2. Ejecuta el programa:
   ```bash
   ./test
   ```
   Si todo está bien, deberías ver:
   ```
   Resultado: 42
   ```
