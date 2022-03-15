#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "serversync.h"

int main() {
  messaggio m;
  /* Coda "OK TO SEND" */

  key_t ok_key = ftok(".", 'o');

  int ok_id = msgget(ok_key, 0644);

  if (ok_id < 0) {
    perror("CLIENT:Errore creazione coda ok-to-send");
    exit(1);
  }

  /* Coda "REQUEST TO SEND" */

  key_t req_key = ftok(".", 'r');

  int req_id = msgget(req_key, 0644);

  if (req_id < 0) {
    perror("CLIENT: Errore creazione coda request-to-send");
    exit(1);
  }

  srand(getpid());

  for (int i = 0; i < 2; i++) {
    int val = rand() % 100;

    printf("[%d] Client: invio val=%d\n", getpid(), val);

    m.val = val;
    m.type = 1;

    /* NOTA: sarà la funzione send_sinc() a scambiare i messaggi
             "REQUEST TO SEND" e "OK TO SEND" con uno dei server.
    */

    send_sinc(ok_id, req_id, &m);

    sleep(2);
  }

  return 0;
}