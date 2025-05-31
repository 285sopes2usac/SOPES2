# Compilación de un Kernel de Linux
## 0. Sistema Operativo
[Descarga de 6.12.31 Linux Mint 22.1.1](https://linuxmint.com/edition.php?id=320)

## 1. Extraer el código fuente del kernel  
[Descargar Kernel de Linux](https://www.kernel.org) (Descargar version longterm 6.12.31)

Extrae el código fuente del kernel de Linux:
```sh
tar -xvf linux-6.12.31.tar.xz
```

## 2. Instalar dependencias necesarias  
Asegúrate de instalar los paquetes esenciales para la compilación del kernel:  

```sh
sudo apt -y install build-essential libncurses-dev bison flex libssl-dev libelf-dev fakeroot
```

## 3. Configurar el kernel 

Generar configuracion basada en el kernel actual
```sh
make localmodconfig
```
Nota: Es posible que durante la creacion del archivo .config, aparezcan opciones de agregar nuevos modulos/drivers que no estan presentes en la configuracion actual del sistema, la opcion segura es usar "N" para no agregar nueva configuracion.


Desactiva las claves de confianza del sistema para evitar problemas con la firma de módulos:  

```sh
scripts/config --disable SYSTEM_TRUSTED_KEYS && \
scripts/config --disable SYSTEM_REVOCATION_KEYS && \
scripts/config --set-str CONFIG_SYSTEM_TRUSTED_KEYS "" && \
scripts/config --set-str CONFIG_SYSTEM_REVOCATION_KEYS ""
```

## 4. Compilar el kernel  
Ejecuta la compilación utilizando `fakeroot` y habilita el uso de 4 núcleos para acelerar el proceso:  

```sh
fakeroot make -j4
```

## 5. Instalar Kernel
```sh
sudo make modules_install
sudo make install
```

## 6. Cambiar Kernel por defecto
Una vez compilado e instalado el kernel, es posible que este no sea la opcion por defecto para iniciar el sistema, hay varias formas de cambiar esto:

### Opcion 1. Habilitar el menu de inicio
Usa tu editor favorito para editar /etc/default/grub y cambia el archivo para que se vea asi
```shell

# If you change this file, run 'update-grub' afterwards to update
# /boot/grub/grub.cfg.
# For full documentation of the options in this file, see:
#   info -f grub -n 'Simple configuration'

GRUB_DEFAULT=saved
GRUB_TIMEOUT_STYLE=menu
GRUB_TIMEOUT=10
GRUB_SAVEDEFAULT=true
GRUB_DISTRIBUTOR=`( . /etc/os-release; echo ${NAME:-Ubuntu} ) 2>/dev/null || echo Ubuntu`
GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"
GRUB_CMDLINE_LINUX=""
```

Una vez guardados los cambios, hacemos
```sh
sudo update-grub
```
Y reiniciamos.

### Opcion 2. Cambiar opcion por defecto
Usa tu editor favorito para editar 

Usa el siguiente comando para obtener las opciones del menu disponibles
```sh
sudo grep menuentry /boot/grub/grub.cfg
```

Veras algo como esto:
```sh
menuentry '6.12.31 Linux Mint 22.1 Xfce' --class linuxmint --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-simple-00f49a3a-c03b-4fe6-94a9-11d1df4dd3db' {
submenu 'Advanced options for 6.12.31 Linux Mint 22.1 Xfce' $menuentry_id_option 'gnulinux-advanced-00f49a3a-c03b-4fe6-94a9-11d1df4dd3db' {
        menuentry '6.12.31 Linux Mint 22.1 Xfce, with Linux 6.12.31-steven-jocol-201602938' --class linuxmint --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-6.12.31-steven-jocol-201602938-advanced-00f49a3a-c03b-4fe6-94a9-11d1df4dd3db' {
        menuentry '6.12.31 Linux Mint 22.1 Xfce, with Linux 6.12.31-steven-jocol-201602938 (recovery mode)' --class linuxmint --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-6.12.31-steven-jocol-201602938-recovery-00f49a3a-c03b-4fe6-94a9-11d1df4dd3db' {
        menuentry '6.12.31 Linux Mint 22.1 Xfce, with Linux 6.12.31-steven-gomez-201602938' --class linuxmint --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-6.12.31-steven-gomez-201602938-advanced-00f49a3a-c03b-4fe6-94a9-11d1df4dd3db' {
        menuentry '6.12.31 Linux Mint 22.1 Xfce, with Linux 6.12.31-steven-gomez-201602938 (recovery mode)' --class linuxmint --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-6.12.31-steven-gomez-201602938-recovery-00f49a3a-c03b-4fe6-94a9-11d1df4dd3db' {
        menuentry '6.12.31 Linux Mint 22.1 Xfce, with Linux 6.12.31-38-generic' --class linuxmint --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-6.12.31-38-generic-advanced-00f49a3a-c03b-4fe6-94a9-11d1df4dd3db' {
        menuentry '6.12.31 Linux Mint 22.1 Xfce, with Linux 6.12.31-38-generic (recovery mode)' --class linuxmint --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-6.12.31-38-generic-recovery-00f49a3a-c03b-4fe6-94a9-11d1df4dd3db' {
        menuentry '6.12.31 Linux Mint 22.1 Xfce, with Linux 6.12.31steven-jocol-201602938' --class linuxmint --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-6.12.31steven-jocol-201602938-advanced-00f49a3a-c03b-4fe6-94a9-11d1df4dd3db' {
        menuentry '6.12.31 Linux Mint 22.1 Xfce, with Linux 6.12.31steven-jocol-201602938 (recovery mode)' --class linuxmint --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-6.12.31steven-jocol-201602938-recovery-00f49a3a-c03b-4fe6-94a9-11d1df4dd3db' {
                menuentry 'UEFI Firmware Settings' $menuentry_id_option 'uefi-firmware' {
```

La parte que nos interesa es la cadena que acompaña la palabra ```submenu``` que en este caso es: ```Advanced options for 6.12.31 Linux Mint 22.1 Xfce``` y tambien la cadena que acompañ al ```menuentry``` que nos interesa, por ejemplo, ```6.12.31 Linux Mint 22.1 Xfce, with Linux 6.12.31-steven-gomez-201602938```

Unimos estas dos cadenas con un ">" como separador y obtenemos: ```"Advanced options for 6.12.31 Linux Mint 22.1 Cinnamo>6.12.31 Linux Mint 22.1 Xfce, with Linux 6.12.31-steven-gomez-201602938"```
Usamos esta cadena para GRUB_DEFAULT y guardamos

```sh
GRUB_DEFAULT="Advanced options for 6.12.31 Linux Mint 22.1 Cinnamo>6.12.31 Linux Mint 22.1 Xfce, with Linux 6.12.31-steven-gomez-201602938"
GRUB_TIMEOUT_STYLE=hidden
GRUB_TIMEOUT=0
GRUB_DISTRIBUTOR=`( . /etc/os-release; echo ${NAME:-Ubuntu} ) 2>/dev/null || echo Ubuntu`
GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"
GRUB_CMDLINE_LINUX=""
GRUB_SAVEDEFAULT=true
```

Una vez guardados los cambios, hacemos
```sh
sudo update-grub
```
Y reiniciamos.

## Notas  
- Asegúrate de estar en el directorio correcto antes de ejecutar los comandos.  
- Puedes ajustar `-j4` según la cantidad de núcleos de tu CPU para optimizar el rendimiento.  
- La compilación puede tardar varios minutos o incluso horas dependiendo del hardware.
- Para mantener las cosas simples, copia y guarda el .config generado con el kernel original de Linux Mint y usalo para compilaciones posteriores

