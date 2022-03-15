#include <sys/types.h>

#define RICHIESTE 5
#define CLIENT 3

typedef struct {
  long pid;
  int a;
  int b;
} client_to_server;
/* TBD: Definire una struct per i messaggi dal client al server */

typedef struct {
  long pid;
  int result;
} server_to_client;
/* TBD: Definire una struct per i messaggi dal server al client */

void client(int id_c, int id_s);
void server(int id_c, int id_s);
void *Prodotto(void *);
