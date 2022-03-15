#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "Header.h"

void client(int id_c, int id_s) {

  int k;
  int ret;

  srand(getpid());

  for (k = 0; k < RICHIESTE; k++) {

    /* TBD: Inviare un messaggio di richiesta */

    int v1 = rand() % 101;
    int v2 = rand() % 101;

    printf("Richiesta %d Inviata (%d, %d) [PID=%d]\n\n", k, v1, v2, getpid());
    client_to_server c;
    c.pid = getpid();
    c.a = v1;
    c.b = v2;
    ret =
        msgsnd(id_c, &c, sizeof(client_to_server) - sizeof(long), 0); /* TBD */

    if (ret < 0) {
      perror("Errore invio richiesta client");
      exit(1);
    }

    /* TBD: Ricevere un messaggio di risposta */
    server_to_client s;
    ret = msgrcv(id_s, &s, sizeof(server_to_client) - sizeof(long), getpid(),
                 0); /* TBD */

    if (ret < 0) {
      perror("Errore ricezione risposta client");
      exit(1);
    }

    int v3 = s.result; /* TBD */

    printf("Risposta %d Ricevuta (%d) [PID=%d]\n\n", k, v3, getpid());
  }
}
