#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "p.h"

int main() {

  printf(GREEN("Processo P4 avviato\n"));

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

  int i;

  for (i = 0; i < 3; i++) {

    int ret;

    int g, h, r3;

    messaggio_op m;

    printf("[P4] RECEIVE P1\n");

    ret = msgrcv(id_operandi_1, &m, sizeof(messaggio_op) - sizeof(long), P4, 0);

    if (ret < 0) {
      perror(BLUE("Msgrcv fallita"));
      exit(1);
    }

    g = m.value1;
    h = m.value2;

    printf("[P4] OPERANDI: g=%d, h=%d\n", g, h);

    r3 = g * h;

    printf("[P4] RISULTATO: %d\n", r3);

    messaggio_ris r;
    r.risultato = r3;
    r.type = P4;

    printf("[P4] SEND P1\n");

    ret = msgsnd(id_risultati_1, &r, sizeof(messaggio_ris) - sizeof(long), 0);

    if (ret < 0) {
      perror(BLUE("Msgsnd fallita"));
      exit(1);
    }
  }

  return 0;
}
