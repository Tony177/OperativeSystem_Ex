#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "p.h"

int main() {

  printf(GREEN("Processo P5 avviato\n"));

  key_t queue_operandi_2 = ftok(".", 'c');
  key_t queue_risultati_2 = ftok(".", 'd');

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

    int c, d, r4;

    messaggio_op m;

    printf("[P5] RECEIVE P3\n");

    ret =
        msgrcv(id_operandi_2, &m, sizeof(messaggio_op) - sizeof(long), P5, 0);

    if (ret < 0) {
      perror(BLUE("Msgrcv fallita"));
      exit(1);
    }

    c = m.value1;
    d = m.value2;

    printf("[P5] OPERANDI: c=%d, d=%d\n", c, d);

    r4 = c + d;

    printf("[P5] RISULTATO: %d\n", r4);

    messaggio_ris r;
    r.risultato = r4;
    r.type = P5;

    printf("[P5] SEND P3\n");

    ret =
        msgsnd(id_risultati_2, &r, sizeof(messaggio_ris) - sizeof(long), 0);

    if (ret < 0) {
      perror(BLUE("Msgsnd fallita"));
      exit(1);
    }
  }

  return 0;
}
