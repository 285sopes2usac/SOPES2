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
Una vez compilado e instalado el kernel, es posible que este no sea la opcion por defecto para iniciar el sistema, hay varias formas de cambiar esto pero la mas sencilla es habilitar el menu de inicio avanzado:

### Habilitar el menu de inicio
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

## Notas  
- Asegúrate de estar en el directorio correcto antes de ejecutar los comandos.  
- Puedes ajustar `-j4` según la cantidad de núcleos de tu CPU para optimizar el rendimiento, si tu equipo tiene 2 nucleos, podria ser mas eficiente usar -j2.
- La compilación puede tardar varios minutos o incluso horas dependiendo del hardware.
