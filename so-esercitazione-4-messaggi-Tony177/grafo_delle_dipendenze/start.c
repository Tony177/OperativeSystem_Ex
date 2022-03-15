#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "p.h"

int main() {

  pid_t pid;
  int i;

  key_t queue_operandi_1 = ftok(".", 'a');
  key_t queue_risultati_1 = ftok(".", 'b');
  key_t queue_operandi_2 = ftok(".", 'c');
  key_t queue_risultati_2 = ftok(".", 'd');

  int id_operandi_1 = msgget(queue_operandi_1, IPC_CREAT | 0644);

  if (id_operandi_1 < 0) {
    perror(BLUE("Msgget fallita"));
    exit(1);
  }

  int id_risultati_1 = msgget(queue_risultati_1, IPC_CREAT | 0644);

  if (id_risultati_1 < 0) {
    perror(BLUE("Msgget fallita"));
    exit(1);
  }

  int id_operandi_2 = msgget(queue_operandi_2, IPC_CREAT | 0644);

  if (id_operandi_2 < 0) {
    perror(BLUE("Msgget fallita"));
    exit(1);
  }

  int id_risultati_2 = msgget(queue_risultati_2, IPC_CREAT | 0644);

  if (id_risultati_2 < 0) {
    perror(BLUE("Msgget fallita"));
    exit(1);
  }

  pid = fork();
  if (pid == 0) {
    execl("./p1", "p1", NULL);
    exit(0);
  }

  pid = fork();
  if (pid == 0) {
    execl("./p2", "p2", NULL);
    exit(0);
  }

  pid = fork();
  if (pid == 0) {
    execl("./p3", "p3", NULL);
    exit(0);
  }

  pid = fork();
  if (pid == 0) {
    execl("./p4", "p4", NULL);
    exit(0);
  }

  pid = fork();
  if (pid == 0) {
    execl("./p5", "p5", NULL);
    exit(0);
  }
  
  pid = fork();
  if (pid == 0) {
    execl("./p6", "p6", NULL);
    exit(0);
  }

  for (int i = 0; i < 6; i++)
    wait(NULL);

  msgctl(queue_operandi_1, IPC_RMID, 0);
  msgctl(queue_operandi_2, IPC_RMID, 0);
  msgctl(queue_risultati_1, IPC_RMID, 0);
  msgctl(queue_risultati_2, IPC_RMID, 0);

  return 0;
}
