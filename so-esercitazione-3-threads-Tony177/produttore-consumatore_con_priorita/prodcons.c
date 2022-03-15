#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "prodcons.h"

void inizializza_prod_cons(PriorityProdCons *p) {

  /* TBD: Inizializzare tutte le variabili del monitor */
  p->coda = 0;
  p->testa = 0;
  p->thread_prio_1 = 0;
  p->utilizzo = 0;

  pthread_cond_init(&(p->cv_cons), NULL);
  pthread_cond_init(&(p->cv_prod), NULL);
  pthread_mutex_init(&(p->mutex), NULL);
}

void rimuovi_prod_cons(PriorityProdCons *p) {

  pthread_mutex_destroy(&(p->mutex));
  pthread_cond_destroy(&(p->cv_prod));
  pthread_cond_destroy(&(p->cv_cons));
  /* TBD: Disattivare mutex e variabili condition */
}

void produci_alta_prio(PriorityProdCons *p) {

  int value;
  pthread_mutex_lock(&(p->mutex));
  while (p->utilizzo == DIM) {
    p->thread_prio_1++;
    pthread_cond_wait(&(p->cv_prod), &(p->mutex));
    p->thread_prio_1--;
  }
  p->utilizzo++;

  /* TBD: Implementare la sincronizzazione secondo lo schema del
   *      produttore-consumatore con vettore di buffer circolare.
   *
   *      Si introduca nel monitor una variabile "threads_prio_1"
   *      per contare il numero di produttori sospesi ad alta priorità.
   */

  value = rand() % 12;
  p->buffer[p->testa] = value;
  p->testa = (p->testa + 1) % DIM;

  printf("Produzione priorità alta: %d\n", value);

  pthread_cond_signal(&(p->cv_cons));
  pthread_mutex_unlock(&(p->mutex));
}

void produci_bassa_prio(PriorityProdCons *p) {

  int value;
  pthread_mutex_lock(&(p->mutex));
  while (p->utilizzo == DIM || p->thread_prio_1 != 0) {
    pthread_cond_wait(&(p->cv_prod), &(p->mutex));
  }
  /* TBD: Implementare la sincronizzazione secondo lo schema del
   *      produttore-consumatore con vettore di buffer circolare.
   *
   *      Si introduca nel monitor una variabile "threads_prio_2"
   *      per contare il numero di produttori sospesi a bassa priorità.
   */
  p->utilizzo++;
  value = 13 + (rand() % 12);
  p->buffer[p->testa] = value;
  p->testa = (p->testa + 1) % DIM;

  printf("Produzione priorità bassa: %d\n", value);
  pthread_cond_signal(&(p->cv_cons));
  pthread_mutex_unlock(&(p->mutex));
}

void consuma(PriorityProdCons *p) {

  int value;
  pthread_mutex_lock(&(p->mutex));
  while (p->utilizzo == 0) {
    pthread_cond_wait(&(p->cv_cons), &(p->mutex));
  }

  /* TBD: Implementare la sincronizzazione secondo lo schema del
   *      produttore-consumatore con vettore di buffer circolare.
   */

  /* Consumazione */

  value = p->buffer[p->coda]; /* TBD */
  p->coda = (p->coda + 1) % DIM;
  printf("Consumato valore %d\n", value);
  p->utilizzo--;
  pthread_cond_signal(&(p->cv_prod));
  pthread_mutex_unlock(&(p->mutex));
}
