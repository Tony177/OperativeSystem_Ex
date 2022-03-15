#ifndef _PRODCONS_H_
#define _PRODCONS_H_

#include "monitor_hoare.h"

#define DIM 10
#define CV_CONS 0
#define CV_PROD 1

typedef struct {
  int vettore[DIM];
  Monitor m;
  int testa;
  int coda;
  int totale_elementi;
} ProdCons;

void inizializza(ProdCons *p);
void consuma(ProdCons *p, int *val_1, int *val_2);
void produci(ProdCons *p, int val);
void rimuovi(ProdCons *p);

#endif
