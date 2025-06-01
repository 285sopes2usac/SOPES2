#include <linux/module.h> // Necesario para módulos de kernel
#include <linux/init.h> // Necesario para funciones de inicialización y limpieza
#include <linux/input.h> // Necesario para manejar dispositivos de entrada
#include <linux/proc_fs.h> // Necesario para manejar el sistema de archivos /proc
#include <linux/uaccess.h> // Necesario para funciones de acceso a memoria de usuario

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Steven Jocol");
MODULE_DESCRIPTION("Módulo para mover el mouse virtualmente");
MODULE_VERSION("0.1");

static struct input_dev *virtual_mouse;
static struct proc_dir_entry *proc_entry;

#define PROC_NAME "vmouse_control"
#define BUF_LEN  64


/**
Funcion que se activa cuando se escribe en el archivo /proc/vmouse_control
@param file puntero al archivo proc
@param buffer puntero al buffer de datos que se escriben, pertenece al espacio de usuario
@param count cantidad de bytes que se escriben
@param pos posición actual en el archivo
 */
static ssize_t mouse_proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *pos)
{
    char buf[BUF_LEN];
    int dx, dy;

    // Verificamos si el tamaño del buffer es válido,  se espera uno de 64 o menos, si fuera mas grande, no lo aceptamos
    if (count > BUF_LEN - 1)
        return -EINVAL;

    // Copiamos el buffer de usuario al buffer del kernel
    if (copy_from_user(buf, buffer, count))
        return -EFAULT;

    buf[count] = '\0'; // Agregamos un nulo al final del buffer, este valor especial indica el final de una cadena en C


    /** sscanf: recibe un buffer de datos y los interpreta segun el valor que se indica
    en este caso, mi cadena trae dos enteros separados por un espacio, los valores dx y dy
    y los quiero guardar en mi variable dx y dy, por eso paso su referencia */
    int parsed = sscanf(buf, "%d %d", &dx, &dy);
    if (parsed < 2)
        return -EINVAL;

    // Movemos nuestro mouse virtual de manera relativa en X y Y con los valores recibidos
    input_report_rel(virtual_mouse, REL_X, dx);
    input_report_rel(virtual_mouse, REL_Y, dy);
    input_sync(virtual_mouse); // Despacha los eventos del mouse al sistema

    pr_info("Mouse movido: X=%d, Y=%d\n", dx, dy);
    return count;
}

static const struct proc_ops proc_file_ops = {
    .proc_write = mouse_proc_write, // Asociamos nuestra función de escritura
};



/** Funcion de Carga del Mouse virtual */
static int __init mouse_init(void)
{
    int err;

    virtual_mouse = input_allocate_device(); // Solicitamos espacio en memoria para nuestro dispositivo virtual
    if (!virtual_mouse) {
        pr_err("No se pudo alocar el dispositivo input\n");
        return -ENOMEM;
    }

    virtual_mouse->name = "virtual_mouse"; // Le asignamos un nombre
    virtual_mouse->phys = "vmd/input0"; // Le damos una ruta "fisica" virtual
    virtual_mouse->id.bustype = BUS_USB; // Simularemos un bug virtual
    virtual_mouse->id.vendor  = 0x0007; // ID de vendedor ficticio
    virtual_mouse->id.product = 0x0009; // ID de producto ficticio
    virtual_mouse->id.version = 0x001; // Versión ficticia del dispositivo

    __set_bit(EV_REL, virtual_mouse->evbit); // Indicamos que el dispositivo soporta eventos relativos
    __set_bit(REL_X, virtual_mouse->relbit); // Soporta movimiento en el eje X
    __set_bit(REL_Y, virtual_mouse->relbit); // Soporta movimiento en el eje Y
    __set_bit(EV_KEY, virtual_mouse->evbit); // Indicamos que el dispositivo soporta eventos de teclas
    __set_bit(BTN_LEFT, virtual_mouse->keybit); // Soporta el botón izquierdo del mouse

    err = input_register_device(virtual_mouse); // Registramos el dispositivo input
    
    if (err) {
        pr_err("No se pudo registrar el dispositivo input\n");
        input_free_device(virtual_mouse);
        return err;
    }

    proc_entry = proc_create(
        PROC_NAME, // Creamos una entrada en /proc
        0222, // Permisos de escritura
        NULL, // Directorio padre, NULL significa raíz
        &proc_file_ops // Estructura de operaciones del archivo, para asociar nuestra funcion al archivo proc
    );

    if (!proc_entry) {
        input_unregister_device(virtual_mouse);
        pr_err("No se pudo crear entrada /proc\n");
        return -ENOMEM;
    }

    pr_info("Módulo cargado: dispositivo virtual_mouse listo\n");
    return 0;
}


/** Funcion de Descarga del Mouse */
static void __exit mouse_exit(void)
{
    proc_remove(proc_entry);
    input_unregister_device(virtual_mouse);
    pr_info("Módulo descargado: mouse virtual removido\n");
}

module_init(mouse_init);
module_exit(mouse_exit);
