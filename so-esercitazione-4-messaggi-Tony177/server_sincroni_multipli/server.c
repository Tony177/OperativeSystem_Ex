#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "serversync.h"

int main() {

  key_t ok_key = ftok(".", 'o');

  int ok_id = msgget(ok_key, 0644);

  if (ok_id < 0) {
    perror("SERVER: Errore creazione coda messaggi");
    exit(1);
  }

  key_t req_key = ftok(".", 'r');

  int req_id = msgget(req_key, 0644);

  if (req_id < 0) {
    perror("SERVER: Errore creazione coda messaggi");
    exit(1);
  }

  key_t msg_key = ftok(".", 'm');

  int msg_id = msgget(msg_key, IPC_CREAT | 0644);

  if (msg_id < 0) {
    perror("SERVER: Errore creazione coda messaggi");
    exit(1);
  }

  for (int i = 0; i < 4; i++) {
    messaggio msg;

    receive_sinc(msg_id, ok_id, req_id, &msg);

    printf("[%d] Server: ricevuto val=%d\n", getpid(), msg.val);
  }

  /* Dà ai client alcuni secondi di tempo per ricevere le risposte,
   * prima di deallocare la coda privata */
  sleep(3);
  msgctl(msg_id, IPC_RMID, NULL);

  return 0;
}