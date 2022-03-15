#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "p.h"

int main() {

  printf(GREEN("Processo P3 avviato\n"));

  key_t queue_operandi_1 = ftok(".", 'a');
  key_t queue_risultati_1 = ftok(".", 'b');
  key_t queue_operandi_2 = ftok(".", 'c');
  key_t queue_risultati_2 = ftok(".", 'd');

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

  int id_operandi_2 = msgget(queue_operandi_2, 0644);

  if (id_operandi_2 < 0) {
    perror(BLUE("Msgget fallita"));
    exit(1);
  }

  int id_risultati_2 = msgget(queue_risultati_2, 0644);

  if (id_risultati_2 < 0) {
    perror(BLUE("Msgget fallita"));
    exit(1);
  }

  int i;

  for (i = 0; i < 3; i++) {

    int ret;

    int c, d, e, f, r2, r4, r5;
    messaggio_op m;

    printf("[P3] RECEIVE P1\n");

    ret = msgrcv(id_operandi_1, &m, sizeof(messaggio_op) - sizeof(long), P3, 0);

    if (ret < 0) {
      perror(BLUE("Msgrcv fallita"));
      exit(1);
    }

    c = m.value1;
    d = m.value2;
    e = m.value3;
    f = m.value4;

    printf("[P3] OPERANDI: c=%d, d=%d, e=%d, f=%d\n", c, d, e, f);

    m.type = P5;
    m.value1 = c;
    m.value2 = d;

    printf("[P3] SEND P5\n");

    ret = msgsnd(id_operandi_2, &m, sizeof(messaggio_op) - sizeof(long), 0);

    if (ret < 0) {
      perror(BLUE("Msgsnd fallita"));
      exit(1);
    }

    m.value1 = e;
    m.value2 = f;
    m.type = P6;
    printf("[P3] SEND P6\n");

    ret = msgsnd(id_operandi_2, &m, sizeof(messaggio_op) - sizeof(long), 0);

    if (ret < 0) {
      perror(BLUE("Msgsnd fallita"));
      exit(1);
    }

    messaggio_ris r;

    printf("[P3] RECEIVE P5\n");

    ret =
        msgrcv(id_risultati_2, &r, sizeof(messaggio_ris) - sizeof(long), P5, 0);

    if (ret < 0) {
      perror(BLUE("Msgrcv fallita"));
      exit(1);
    }

    r4 = r.risultato;

    printf("[P3] RECEIVE P6\n");

    ret =
        msgrcv(id_risultati_2, &r, sizeof(messaggio_ris) - sizeof(long), P6, 0);

    if (ret < 0) {
      perror(BLUE("Msgrcv fallita"));
      exit(1);
    }

    r5 = r.risultato;

    printf("[P3] RISULTATI INTERMEDI: r4=%d, r5=%d\n", r4, r5);

    r2 = r4 * r5;

    printf("[P3] RISULTATO: %d\n", r2);

    r.risultato = r2;
    r.type = P3;

    printf("[P3] SEND P1\n");

    ret = msgsnd(id_risultati_1, &r, sizeof(messaggio_ris) - sizeof(long), 0);

    if (ret < 0) {
      perror(BLUE("Msgsnd fallita"));
      exit(1);
    }
  }

  return 0;
}
