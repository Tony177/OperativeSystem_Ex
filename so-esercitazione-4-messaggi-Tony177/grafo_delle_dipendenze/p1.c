#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>

#include "p.h"

int main() {

  printf(GREEN("Processo P1 avviato\n"));

  key_t queue_operandi_1 = ftok(".", 'a');
  key_t queue_risultati_1 = ftok(".", 'b');

  int id_operandi_1 = msgget(queue_operandi_1, 0644);

  if (id_operandi_1 < 0) {
    perror(BLUE("Msgget fallita"));
    exit(1);
  }

  int id_risultati_1 = msgget(queue_risultati_1, 0644);

  if (id_risultati_1 < 0) {
    perror(BLUE("Msgget fallita"));
    exit(1);
  }

  srand(time(NULL));

  int i;

  for (i = 0; i < 3; i++) {

    int ret;

    int a = rand() % 10;
    int b = rand() % 10;
    int c = rand() % 10;
    int d = rand() % 10;
    int e = rand() % 10;
    int f = rand() % 10;
    int g = rand() % 10;
    int h = rand() % 10;

    int r1, r2, r3, risultato;

    printf("[P1] OPERANDI: a=%d, b=%d, c=%d, d=%d, e=%d, f=%d, g=%d, h=%d\n", a,
           b, c, d, e, f, g, h);

    printf("[P1] SEND P2\n");
    messaggio_op m;
    m.value1 = a;
    m.value2 = b;
    m.type = P2;
    ret = msgsnd(id_operandi_1, &m, sizeof(messaggio_op) - sizeof(long), 0);

    if (ret < 0) {
      perror(BLUE("Msgsnd fallita"));
      exit(1);
    }
    m.value1 = c;
    m.value2 = d;
    m.value3 = e;
    m.value4 = f;
    m.type = P3;

    printf("[P1] SEND P3\n");

    ret = msgsnd(id_operandi_1, &m, sizeof(messaggio_op) - sizeof(long), 0);

    if (ret < 0) {
      perror(BLUE("Msgsnd fallita"));
      exit(1);
    }

    m.value1 = g;
    m.value2 = h;
    m.type = P4;

    printf("[P1] SEND P4\n");

    ret = msgsnd(id_operandi_1, &m, sizeof(messaggio_op) - sizeof(long), 0);

    if (ret < 0) {
      perror(BLUE("Msgsnd fallita"));
      exit(1);
    }

    printf("[P1] RECEIVE P2\n");
    messaggio_ris ris;
    ret = msgrcv(id_risultati_1, &ris, sizeof(messaggio_ris) - sizeof(long), P2,
                 0);

    if (ret < 0) {
      perror(BLUE("Msgrcv fallita"));
      exit(1);
    }

    r1 = ris.risultato;

    printf("[P1] RECEIVE P3\n");

    ret = msgrcv(id_risultati_1, &ris, sizeof(messaggio_ris) - sizeof(long), P3,
                 0);

    if (ret < 0) {
      perror(BLUE("Msgrcv fallita"));
      exit(1);
    }

    r2 = ris.risultato;

    printf("[P1] RECEIVE P4\n");

    ret = msgrcv(id_risultati_1, &ris, sizeof(messaggio_ris) - sizeof(long), P4,
                 0);

    if (ret < 0) {
      perror(BLUE("Msgrcv fallita"));
      exit(1);
    }

    r3 = ris.risultato;

    printf("[P1] RISULTATI INTERMEDI: r1=%d, r2=%d, r3=%d\n", r1, r2, r3);

    risultato = r1 + r2 + r3;

    printf("[P1] RISULTATO: %d\n", risultato);
  }

  return 0;
}
