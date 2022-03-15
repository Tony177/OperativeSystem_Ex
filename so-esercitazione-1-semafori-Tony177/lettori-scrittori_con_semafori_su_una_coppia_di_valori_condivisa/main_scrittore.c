#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#include "buffer.h"

int main() {
  printf("[%d] Avvio scrittore\n", getpid());

  key_t chiave_shm = ftok("./buffer.c", 'a');

  int shm_id = shmget(chiave_shm, sizeof(buffer), 0);

  if (shm_id < 0) {
    perror("Errore creazione shared memory");
    exit(1);
  }

  buffer *b = shmat(shm_id, NULL, 0);

  if (b == (void *)-1) {
    perror("Errore attach shared memory");
    exit(1);
  }

  key_t chiave_sem = ftok("./buffer.c", 'b');

  int sem_id = semget(chiave_sem, 2, 0);

  if (sem_id < 0) {
    perror("Errore semget()");
    exit(1);
  }

  srand(getpid());

  for (int i = 0; i < 5; i++) {

    int val_1 = rand() % 10;
    int val_2 = rand() % 10;

    scrivi_buffer(b, sem_id, val_1, val_2);
  }

  return 0;
}