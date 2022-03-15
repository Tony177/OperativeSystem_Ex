#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

int main() {
  Monitor M;
  init_monitor(&M, 2);
  /* TBD: Creare una variabile M di tipo "Monitor", e inizializzarla con
   * init_monitor() */

  key_t id_meteo = shmget(IPC_PRIVATE, sizeof(Buffer), IPC_CREAT | 0664);
  Buffer *buf = (Buffer *)(shmat(id_meteo, 0, 0));
  srand(time(NULL));
  buf->meteo.pioggia = rand() % 2;              // Tra 0 e 1
  buf->meteo.umidita = rand() % 101;            // Tra 0 e 100
  buf->meteo.temperatura = (rand() % 101) - 50; // Tra -50 e 50
  buf->num_lettori = 0;
  /* TBD: inizializzare la struttura Buffer */

  pid_t pid;

  int k;
  for (k = 0; k < NUM_UTENTI; k++) {

    pid = fork();
    if (pid == 0) {
      Utente(&M, buf);
      exit(0);
    } else if (pid < 0) {
      perror("fork");
    }
  }

  pid = fork();
  if (pid == 0) {
    Servizio(&M, buf);
    exit(0);
  } else if (pid < 0) {
    perror("fork");
  }

  int status;
  for (k = 0; k < NUM_UTENTI + 1; k++) {
    pid = wait(&status);
    if (pid == -1)
      perror("errore");
  }

  shmctl(id_meteo, IPC_RMID, 0);

  remove_monitor(&M);
  /* TBD: Deallocare la variabile Monitor con remove_monitor() */

  return 0;
}
