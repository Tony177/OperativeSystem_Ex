#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include "coda_richieste.h"

coda_richieste *inizializza_coda() {
  int shm_id;
  int sem_id;
  coda_richieste *c;

  sem_id = semget(IPC_PRIVATE, 2,
                  IPC_CREAT | 0644); /* TBD: Creazione vettore semafori */

  if (sem_id < 0) {
    perror("Errore creazione semafori");
    exit(1);
  }
  semctl(sem_id, MUTEXU, SETVAL, 0);
  semctl(sem_id, SYNC, SETVAL, 1);
  /* TBD: Inizializzazione vettore semafori */

  shm_id = shmget(IPC_PRIVATE, sizeof(coda_richieste),
                  IPC_CREAT | 0644); /* TBD: Creazione shared memory */

  if (shm_id < 0) {
    perror("Errore creazione SHM coda richieste");
    exit(1);
  }

  c = shmat(shm_id, NULL, 0); /* TBD: Attach shared memory */

  if (c == (void *)-1) {
    perror("Errore attach SHM");
    exit(1);
  }
  c->coda = 0;
  c->testa = 0;
  c->sem_id = sem_id;
  c->shm_id = shm_id;

  /* TBD: Inizializzazione shared memory */
  return c;
}

void preleva_richiesta(coda_richieste *c, richiesta *r) {

  /* TBD: Inizio consumazione */
  if (c->coda == c->testa) {
    Wait_Sem(c->sem_id, MUTEXU);
  }
  Wait_Sem(c->sem_id, SYNC);

  printf("[%d] Consumazione in coda: %d\n", getpid(), c->coda);

  r->posizione = c->vettore[c->coda].posizione; /* TBD */
  r->processo = c->vettore[c->coda].processo;   /* TBD */
  c->coda = (c->coda + 1) % DIM;
  if ((c->testa + 1) % DIM == c->coda)
    Signal_Sem(c->sem_id, MUTEXU);
  Signal_Sem(c->sem_id, SYNC);
  /* TBD: Fine consumazione */
}

void inserisci_richiesta(coda_richieste *c, richiesta *r) {
  /* TBD: Inizio produzione */

  if ((c->testa + 1) % DIM == c->coda)
    Wait_Sem(c->sem_id, MUTEXU);
  Wait_Sem(c->sem_id, SYNC);
  printf("[%d] Produzione in testa: %d\n", getpid(), c->testa);

  /* TBD */ c->vettore[c->testa].posizione = r->posizione;
  /* TBD */ c->vettore[c->testa].processo = r->processo;
  c->testa = (c->testa + 1) % DIM;
  if (c->testa == (c->coda + 1) % DIM)
    Signal_Sem(c->sem_id, MUTEXU);
  Signal_Sem(c->sem_id, SYNC);
  /* TBD: Fine produzione */
}

void *rimuovi_coda(coda_richieste *c) {
  semctl(c->sem_id, 0, IPC_RMID);
  shmctl(c->shm_id, IPC_RMID, NULL);
}