#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "prodcons.h"

void *produttore_alta_priorita(void *p) {

  int i;
  PriorityProdCons *p1 = (PriorityProdCons *)p;

  for (i = 0; i < 3; i++) {

    /* TBD: Chiamare il metodo di produzione alta priorità */
    produci_alta_prio(p1);
    sleep(2);
  }

  pthread_exit(NULL);
}

void *produttore_bassa_priorita(void *p) {

  int i;
  PriorityProdCons *p1 = (PriorityProdCons *)p;
  for (i = 0; i < 3; i++) {

    /* TBD: Chiamare il metodo di produzione bassa priorità */
    produci_bassa_prio(p1);
    sleep(1);
  }

  pthread_exit(NULL);
}

void *consumatore(void *p) {

  int i;
  PriorityProdCons *p1 = (PriorityProdCons *)p;
  for (i = 0; i < 12; i++) {

    /* TBD: Chiamare il metodo consuma() */
    consuma(p1);
    sleep(1);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  pthread_t threads[5];
  pthread_attr_t attr;

  int rc;
  int i;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  srand(time(NULL));

  PriorityProdCons *prodcons = (PriorityProdCons *)malloc(
      sizeof(PriorityProdCons)); /* TBD: Allocare un oggetto monitor */

  inizializza_prod_cons(prodcons);
  /* TBD: Inizializzare l'oggetto monitor */
  for (i = 0; i < 3; i++) {
    pthread_create(&threads[i], &attr, produttore_bassa_priorita,
                   (void *)prodcons);
  }
  pthread_create(&threads[3], &attr, produttore_alta_priorita,
                 (void *)prodcons);
  pthread_create(&threads[4], &attr, consumatore, (void *)prodcons);
  /* TBD: Creare 3 thread produttore bassa priorità,
   *      1 thread produttore alta priorità,
   *      e 1 thread consumatore.
   */
  for (i = 0; i < 5; i++)
    pthread_join(threads[i], NULL);
  /* TBD: Effettuare la join con i thread */
  rimuovi_prod_cons(prodcons);
  /* TBD: Richiamare il distrutture dell'oggetto monitor */
}
