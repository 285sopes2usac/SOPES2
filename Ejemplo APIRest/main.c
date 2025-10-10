#include "mongoose.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

/** mg_http_message struct tiene:
    hm->method → método (GET, POST, etc.)
    hm->uri → la ruta solicitada (por ejemplo, /api/time/get)
    hm->body → el cuerpo del mensaje
    hm->query → parámetros del query string
*/

/** mg_strcmp es similar a strcmp pero no necesita el final de cadena en \0
    por lo que funciona con los URI de mongoose. Sirve para comparar dos cadenas, 
    si devuelve 0, son iguales
*/

/** snprintf sirve para copiar el contenido de hm->body a un buffer de C
    sin que se desborde ni que dependa  */






// Manejador de eventos HTTP
static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {


    struct mg_http_message *hm = (struct mg_http_message *) ev_data;

    // Manejar GET request
    if (mg_strcmp(hm->uri, mg_str("/api/time/get")) == 0) {
      mg_http_reply(
        c, 200, 
        "Content-Type: application/json\r\n", 
        "{ \"time\": %lu }\n", time(NULL)
      );
    }
    
    // Manejar POST request
    else if (mg_strcmp(hm->uri, mg_str("/api/data")) == 0) {
      // Extraer datos del cuerpo del POST
      char body[100];
      snprintf(body, sizeof(body), "%.*s", (int)hm->body.len, hm->body.buf);
      
      // Manipular informacion del body JSON

      // Responder con los datos recibidos
      mg_http_reply(
        c, 200, 
        "Content-Type: application/json\r\n",
        "{ \"message\": \"POST received\", \"data\": \"%s\" }\n", body);
    } else {
      // Si no es /api/time/get ni /api/data, intenta servir archivos desde el directorio web_root
      struct mg_http_serve_opts opts = { .root_dir = "./web_root/" };
      mg_http_serve_dir(c, hm, &opts);
    }
  }
}

int main(void) {
  struct mg_mgr mgr;  // Declarar el gestor de eventos
  mg_mgr_init(&mgr);  // Inicializar el gestor de eventos

  // Escuchar en el puerto 8090
  mg_http_listen(&mgr, "http://0.0.0.0:8090", ev_handler, NULL);

  // Bucle principal del servidor
  for (;;) {
    mg_mgr_poll(&mgr, 1000);  // Bucle que procesa eventos
  }

  // Liberar recursos (aunque en este caso nunca se llega aquí)
  mg_mgr_free(&mgr);
  return 0;
}