#ifndef _P_H_
#define _P_H_

#include "colors.h"
// Aggiunto colors.h per rendere il programma più leggibile

#define P1 1 
#define P2 2
#define P3 3 
#define P4 4 
#define P5 5 
#define P6 6 

typedef struct {
  long type;
  int value1;
  int value2;
  int value3;
  int value4;
} messaggio_op;

typedef struct {
  long type;
  int risultato;
} messaggio_ris;



#endif // _P_H_
