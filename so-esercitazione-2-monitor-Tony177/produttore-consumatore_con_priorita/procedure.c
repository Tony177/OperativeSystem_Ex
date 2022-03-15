/*IMPLEMENTAZIONE DELLE PROCEDURE*/

#include "procedure.h"
#include "monitor_signal_continue.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* TBD: Definire delle macro per identificare le variabili condition */
#define CV_PROD 0
#define CV_CONS 1

void inizializza_prod_cons(PriorityProdCons *p) {
  p->testa = 0;
  p->coda = 0;
  p->contatore = 0;
  p->num_prio_h = 0;
  init_monitor(&(p->m), 2);
  /* TBD: Inizializzare le variabili della struttura (testa, coda, ...) */

  /* TBD: Inizializzare il sotto-oggetto "Monitor", chiamando init_monitor(...).
          Scegliere accuratamente il numero di variabili condition */
}

void rimuovi_prod_cons(PriorityProdCons *p) {
  remove_monitor(&(p->m));
  /* TBD: Deallocare il sotto-oggetto "Monitor", chiamando remove_Monitor(...).
   */
}

void produci_alta_prio(PriorityProdCons *p) {

  int value;
  enter_monitor(&(p->m));
  /* TBD: Effettuare l'ingresso nel monitor */
  // Aumenta il numero di produttori
  printf("Produttore 1 entrato nel monitor...\n");
  while (p->contatore == DIM) {
    p->num_prio_h++;
    wait_condition(&(p->m), CV_PROD);
    p->num_prio_h--;
  }
  /* TBD: Sospendere qui il processo se il vettore di buffer è pieno */

  // Produco il valore
  value = rand() % 12;

  // Modifico il buffer
  p->buffer[p->testa] = value;
  p->testa = (p->testa + 1) % DIM;
  p->contatore++;

  // Il buffer non è vuoto
  printf("Produttore 1 con pid %d ha prodotto %d\n", getpid(), value);
  signal_condition(&(p->m), CV_CONS);
  /* TBD: Effettuare la signal_condition per i consumatori in attesa */
  leave_monitor(&(p->m));
  /* TBD: Uscire dal monitor */
}

void produci_bassa_prio(PriorityProdCons *p) {

  int value;
  enter_monitor(&(p->m));
  /* TBD: Effettuare l'ingresso nel monitor */
  while (p->contatore == DIM || p->num_prio_h != 0) {
    wait_condition(&(p->m), CV_PROD);
  }
  // Aumenta il numero di produttori
  printf("Produttore 2 entrato nel monitor...\n");

  /* TBD: Sospendere qui il processo se il vettore di buffer è pieno */

  // Produco il valore
  value = 13 + (rand() % 12);

  // Modifico il buffer
  p->buffer[p->testa] = value;
  p->testa = (p->testa + 1) % DIM;
  p->contatore++;

  // Il buffer non è vuoto
  printf("Produttore 2 con pid %d ha prodotto %d\n", getpid(), value);
  signal_condition(&(p->m), CV_CONS);
  /* TBD: Effettuare la signal_condition per i consumatori in attesa */
	leave_monitor(&(p->m));
  /* TBD: Uscire dal monitor */
}

void consuma(PriorityProdCons *p) {

  int value;
  enter_monitor(&(p->m));
  /* TBD: Effettuare l'ingresso nel monitor */
  while (p->contatore == 0) {
    wait_condition(&(p->m), CV_CONS);
  }
  /* TBD: Sospendere qui il processo se il vettore di buffer è vuoto */

  value = p->buffer[p->coda];
  p->coda = (p->coda + 1) % DIM;
  p->contatore--;

  printf("Consumatore con pid %d ha consumato valore %d\n", getpid(), value);

  signal_condition(&(p->m), CV_PROD);
  leave_monitor(&(p->m));
  /* TBD: Effettuare la signal_condition per attivare un produttore.
   *      Si attivi un produttore di priorità alta se presente, altrimenti,
   *      si attivi un produttore di priorità bassa.
   *
   * 		Per determinare se ci sono produttori ad alta priorità in
   * attesa, si introduca una variabile "num_lettori_alta_priorita", che i
   * produttori incrementano subito prima di fare wait_condition(), e che
   * decrementano subito dopo aver eseguito wait_condition().
   *
   * 		In alternativa, utilizzare la funzione "queue_condition()".
   */

  /* TBD: Uscire dal monitor */
}
