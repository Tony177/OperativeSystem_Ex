#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

  /* Coda "OK TO SEND" */
  pid_t pid;
  key_t ok_key = ftok(".", 'o');

  int ok_id = msgget(ok_key, IPC_CREAT | 0644);

  if (ok_id < 0) {
    perror("Errore creazione coda messaggi");
    exit(1);
  }

  /* Coda "REQUEST TO SEND" */

  key_t req_key = ftok(".", 'r');

  int req_id = msgget(req_key, IPC_CREAT | 0644);

  if (req_id < 0) {
    perror("Errore creazione coda messaggi");
    exit(1);
  }

  for (int i = 0; i < 2; i++) {

    pid = fork();
    if (pid == 0) {
      execl("./server", "server",NULL);
      exit(0);
    }
  }

  for (int i = 0; i < 4; i++) {
    pid = fork();
    if (pid == 0) {
      execl("./client", "client",NULL);
      exit(0);
    }
  }

  printf("[%d] In attesa di terminazione...\n", getpid());
  for (int i = 0; i < 6; i++)
    wait(NULL);

  printf("[%d] Deallocazione code\n", getpid());

  msgctl(ok_id, IPC_RMID, NULL);
  msgctl(req_id, IPC_RMID, NULL);

  return 0;
}