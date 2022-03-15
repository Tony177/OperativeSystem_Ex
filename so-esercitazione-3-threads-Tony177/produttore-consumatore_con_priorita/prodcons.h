#ifndef __PROCEDURE_H
#define __PROCEDURE_H

#include <pthread.h>

#define DIM 3

typedef struct {

  int buffer[DIM];
  int testa;
  int coda;

  pthread_cond_t cv_prod;
  pthread_cond_t cv_cons;
  pthread_mutex_t mutex;
  int thread_prio_1; // Priorità alta
  int utilizzo; // Quanto è pieno
  /* TBD: Aggiungere ulteriori variabili per la sincronizzazione */

} PriorityProdCons;

void inizializza_prod_cons(PriorityProdCons *p);
void produci_alta_prio(PriorityProdCons *p);
void produci_bassa_prio(PriorityProdCons *p);
void consuma(PriorityProdCons *p);
void rimuovi_prod_cons(PriorityProdCons *p);

#endif
