#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Origen del pam_conv
// struct pam_conv {
//     int (*conv)(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr);
//     void *appdata_ptr;
// };

//calloc reserva memoria en el heap y les coloca valor 0

// PAM_SUCCESS es parte de security/pam_appl.h y es una constante

// strdup copia el contenido de en un nuevo espacio de memoria

// PAM_PROMPT_ECHO_OFF es un mensaje que pide informacion secreta, 
// normalmente una contraseña, pero puede ser tambien contraseña de ldap, 
// de kerberos, un pin o token de smartcard o un OTP.

// -lpam y -lpam_misc le dicen al linker donde esta la implementacion real de las cabeceras de pam y pam_misc


static int pam_conv_callback(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr) {
    if (num_msg <= 0) return PAM_CONV_ERR;

    struct pam_response *responses = calloc(num_msg, sizeof(struct pam_response));
    if (!responses) return PAM_CONV_ERR;

    const char *password = (const char *)appdata_ptr;

    for (int i = 0; i < num_msg; i++) {
        if (msg[i]->msg_style == PAM_PROMPT_ECHO_OFF) { // Aqui es donde potencialmente pide la contraseña
            responses[i].resp = strdup(password);
        } else if (msg[i]->msg_style == PAM_PROMPT_ECHO_ON) { // Aqui esta pidiendo texto nromal, no nos interesa responder nada
            responses[i].resp = NULL;
        } else { // Un tipo de mensaje desconocido, error
            free(responses);
            return PAM_CONV_ERR;
        }
    }

    *resp = responses;
    return PAM_SUCCESS;
}

int main() {
    char username[64];
    char password[64];

    printf("Usuario: ");
    scanf("%63s", username);

    printf("Contraseña: ");
    scanf("%63s", password); // Mostramos la entrada pero solo como ejemplo

    struct pam_conv conversation = {pam_conv_callback, (void *)password};
    pam_handle_t *pam_handler = NULL;

    int pam_result = pam_start("login", username, &conversation, &pam_handler);

    if (pam_result != PAM_SUCCESS) {
        fprintf(stderr, "Error iniciando PAM: %s\n", pam_strerror(pam_handler, pam_result));
        return 1;
    }

    pam_result = pam_authenticate(pam_handler, 0); // Validar la contraseña
    if (pam_result == PAM_SUCCESS) {
        pam_result = pam_acct_mgmt(pam_handler, 0); // Validar si la cuenta esta activa
    }

    if (pam_result == PAM_SUCCESS) {
        printf("Autenticación Completa ✅\n");
    } else {
        fprintf(stderr, "Usuario o contraseña incorrectos ❌: %s\n", pam_strerror(pam_handler, pam_result));
    }

    pam_end(pam_handler, pam_result);
    return (pam_result == PAM_SUCCESS ? 0 : 1);
}
