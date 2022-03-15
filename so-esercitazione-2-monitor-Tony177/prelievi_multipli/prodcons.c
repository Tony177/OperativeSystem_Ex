#include <stdio.h>
#include <unistd.h>

#include "prodcons.h"

void inizializza(ProdCons *p) {

  p->totale_elementi = 0;
  p->coda = 0;
  p->testa = 0;
  init_monitor(&(p->m), 2);
}

void consuma(ProdCons *p, int *val_1, int *val_2) {

  enter_monitor(&(p->m));

  printf("[%d] Ingresso consumatore\n", getpid());

  if (p->totale_elementi < 2)
    wait_condition(&(p->m), CV_CONS);

  *val_1 = p->vettore[p->coda];
  p->coda = (p->coda + 1) % DIM;
  p->totale_elementi--;

  printf("[%d] Prima consumazione: val_1=%d\n", getpid(), *val_1);
  signal_condition(&(p->m), CV_PROD);

  *val_2 = p->vettore[p->coda];
  p->coda = (p->coda + 1) % DIM;
  p->totale_elementi--;

  printf("[%d] Seconda consumazione: val_2=%d\n", getpid(), *val_2);
  signal_condition(&(p->m), CV_PROD);

  printf("[%d] Uscita consumatore\n", getpid());

  leave_monitor(&(p->m));
}

void produci(ProdCons *p, int val) {

  enter_monitor(&(p->m));

  printf("[%d] Ingresso produttore\n", getpid());

  if ((p->testa + 1) % DIM == p->coda)
    wait_condition(&(p->m), CV_PROD);

  p->vettore[p->testa] = val;
  p->testa = (p->testa + 1) % DIM;
  p->totale_elementi++;

  printf("[%d] Produzione: val=%d\n", getpid(), val);

  if (p->totale_elementi >= 2) {
    signal_condition(&(p->m), CV_CONS);
  }

  leave_monitor(&(p->m));
  printf("[%d] Uscita produttore\n", getpid());
}

void rimuovi(ProdCons *p) { remove_monitor(&(p->m)); }
