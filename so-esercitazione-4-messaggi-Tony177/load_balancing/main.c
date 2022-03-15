#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "header.h"

int main() {

  int msg_id_balancer;
  int msg_id_server[TOTALE_SERVER];
  int i;
  pid_t p;
  int status;
  int ret;

  msg_id_balancer = msgget(IPC_PRIVATE, IPC_CREAT | 0644);

  if (msg_id_balancer < 0) {

    perror("Errore msgget() coda balancer");
    exit(1);
  }

  for (i = 0; i < TOTALE_SERVER; i++) {

    msg_id_server[i] = msgget(IPC_PRIVATE, IPC_CREAT | 0644);

    if (msg_id_server[i] < 0) {

      perror("Errore msgget() coda server");
      exit(1);
    }
  }

  for (i = 0; i < TOTALE_CLIENT; i++) {
    p = fork();
    if (p == 0) {
      Client(msg_id_balancer);
      exit(0);
    }
  }

  for (i = 0; i < TOTALE_SERVER; i++) {

    p = fork();
    if (p == 0) {
      Server(msg_id_server[i]);
      exit(0);
    }
  }

  p = fork();
  if (p == 0) {
    Balancer(msg_id_balancer, msg_id_server);
    exit(0);
  }

  for (i = 0; i < TOTALE_CLIENT + TOTALE_SERVER + 1; i++) {
    wait(NULL);
  }

  msgctl(msg_id_balancer, IPC_RMID, 0);
  for (i = 0; i < TOTALE_SERVER; i++)
    msgctl(msg_id_server[i], IPC_RMID, 0);

  return 0;
}
