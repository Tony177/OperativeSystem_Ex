#include <stdio.h>
#include <unistd.h>

#include "buffer.h"
#include "semafori.h"

void leggi_buffer(buffer *b, int sem_id, int *val_1, int *val_2) {

  printf("[%d] Inizio lettura\n", getpid());

  Wait_Sem(sem_id, MUTEXL);
  b->num_lettori = b->num_lettori + 1;

  if (b->num_lettori == 1)
    Wait_Sem(sem_id, SYNCH);
  /* TBD: Inizio lettura */
  Signal_Sem(sem_id, MUTEXL);

  printf("[%d] Lettura buffer: val1=%d, val2=%d\n", getpid(), b->val_1,
         b->val_2);

  sleep(1);

  *val_1 = b->val_1;
  *val_2 = b->val_2;

  printf("[%d] Fine lettura\n", getpid());
  Wait_Sem(sem_id, MUTEXL);
  b->num_lettori = b->num_lettori - 1;
  if (b->num_lettori == 0)
    Signal_Sem(sem_id, SYNCH);

  Signal_Sem(sem_id, MUTEXL);
  /* TBD: Fine lettura */
}

void scrivi_buffer(buffer *b, int sem_id, int val_1, int val_2) {

  printf("[%d] Inizio scrittura\n", getpid());

  Wait_Sem(sem_id, SYNCH);
  /* TBD: Inizio scrittura */

  printf("[%d] Scrittura buffer: val1=%d, val2=%d\n", getpid(), val_1, val_2);

  sleep(2);

  b->val_1 = val_1;
  b->val_2 = val_2;
  printf("[%d] Fine scrittura\n", getpid());

  Signal_Sem(sem_id, SYNCH);
  /* TBD: Fine scrittura */
}
