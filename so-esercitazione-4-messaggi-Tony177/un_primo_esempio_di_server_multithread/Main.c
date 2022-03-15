#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "Header.h"

int main() {

  pid_t pidc, pids;
  int id_c, id_s, i;
  int ret;

  id_c = msgget(
      IPC_PRIVATE,
      IPC_CREAT |
          0644); /* Ottenere una coda per le richieste dal client al server */

  if (id_c < 0) {
    perror("Errore allocazione coda");
    exit(1);
  }

  id_s = msgget(
      IPC_PRIVATE,
      IPC_CREAT |
          0644); /* Ottenere una coda per le risposte dal server al client */

  if (id_s < 0) {
    perror("Errore allocazione coda");
    exit(1);
  }

  for (i = 0; i < CLIENT; i++) {
    pidc = fork();
    if (pidc == 0) {
      client(id_c, id_s);
      exit(0);
    } else if (pidc < 0) {
      perror("Errore creazione client");
      exit(1);
    }
    /* TBD: Avviare i processi client, chiamando client() */
  }

  pids = fork();

  if (pids == 0) {
    server(id_c, id_s);
    exit(0);
  }
  if (pids < 0) {
    perror("Errore fork server");
    exit(1);
  }

  /* TBD: Avviare il processo server, chiamando server() */

  for (int i = 0; i < CLIENT; i++)
    wait(NULL);
  /* TBD: Attendere la terminazione dei client */

  /* TBD: Inviare al server il messaggio di terminazione, con i valori {-1, -1}
   */
  client_to_server c;
  c.a = -1;
  c.b = -1;
  c.pid = getpid();
  ret = msgsnd(id_c, &c, sizeof(client_to_server) - sizeof(long), 0); /* TBD */
  if (ret < 0) {
    perror("Errore invio messaggio di terminazione");
    exit(1);
  }

  wait(0);
  msgctl(id_c, IPC_RMID, NULL);
  msgctl(id_s, IPC_RMID, NULL);
  /* TBD: Deallocazione code */

  return 0;
}
