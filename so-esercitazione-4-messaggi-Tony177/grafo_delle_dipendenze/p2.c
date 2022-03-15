#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "p.h"

int main() {

  printf(GREEN("Processo P2 avviato\n"));

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

    int a, b, r1;
    messaggio_op m;

    printf("[P2] RECEIVE P1\n");

    ret = msgrcv(id_operandi_1, &m, sizeof(messaggio_op) - sizeof(long), P2,
                 0);

    if (ret < 0) {
      perror(BLUE("Msgrcv fallita"));
      exit(1);
    }

    a = m.value1;
    b = m.value2;
    printf("[P2] OPERANDI: a=%d, b=%d\n", a, b);

    r1 = a * b;

    printf("[P2] RISULTATO: %d\n", r1);

    messaggio_ris r;
    r.risultato = r1;
    r.type = P2;

    printf("[P2] SEND P1\n");

    ret =
        msgsnd(id_risultati_1, &r, sizeof(messaggio_ris) - sizeof(long), 0);

    if (ret < 0) {
      perror(BLUE("Msgsnd fallita"));
      exit(1);
    }
  }

  return 0;
}
