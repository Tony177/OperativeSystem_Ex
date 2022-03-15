#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#include "buffer.h"

int main() {

  printf("[%d] Creazione shared memory\n", getpid());

  key_t chiave_shm = ftok("./buffer.c", 'a');

  int shm_id = shmget(chiave_shm, sizeof(buffer), IPC_CREAT | 0644);

  if (shm_id < 0) {
    perror("Errore creazione shared memory");
    exit(1);
  }

  buffer *b = shmat(shm_id, NULL, 0);

  if (b == (void *)-1) {
    perror("Errore attach shared memory");
    exit(1);
  }

  b->num_lettori = 0;
  b->val_1 = 0;
  b->val_2 = 0;

  printf("[%d] Creazione semafori\n", getpid());

  key_t chiave_sem = ftok("./buffer.c", 'b');

  int sem_id = semget(chiave_sem, 2, IPC_CREAT | 0644);

  if (sem_id < 0) {
    perror("Errore creazione semafori");
    exit(1);
  }

  semctl(sem_id, MUTEXL, SETVAL, 1);
  semctl(sem_id, SYNCH, SETVAL, 1);

  printf("[%d] Creazione processo scrittore\n", getpid());

  pid_t pid = fork();
  if (pid == 0) {
    execl("./main_scrittore", "main_scrittore", NULL);
    exit(0);
  }

  for (int i = 0; i < 2; i++) {
    pid_t pid = fork();
    printf("[%d] Creazione processo lettore\n", getpid());
    if (pid == 0) {
      execl("./main_lettori", "main_lettori", NULL);
      exit(0);
    }
  }

  printf("[%d] In attesa di terminazione dei processi\n", getpid());

  for (int i = 0; i < 3; i++)
    wait(NULL);

  printf("[%d] Deallocazione risorse\n", getpid());

  shmctl(shm_id, IPC_RMID, NULL);
  semctl(sem_id, 0, IPC_RMID);
}