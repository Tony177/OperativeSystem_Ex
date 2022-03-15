#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "Header.h"

/* Il thread padre condivide l'id della coda
   delle risposte con i figli, tramite una
   variabile globale */

int id_coda_risposte;

void server(int id_c, int id_s) {

  int k = 0;
  int ret;

  id_coda_risposte = id_s;

  while (1) {

    /* TBD: Ricevere un messaggio di richiesta dal client */
    client_to_server *from_client =
        (client_to_server *)malloc(sizeof(client_to_server));
    ret = msgrcv(id_c, from_client, sizeof(client_to_server) - sizeof(long), 0,
                 0); /* TBD */

    if (ret < 0) {
      perror("Errore ricezione richiesta server");
      exit(1);
    }

    if (/* TBD */ from_client->a == -1 && /* TBD */ from_client->b == -1) {

      /* Il processo esce quando si riceve un messaggio di terminazione,
         con i valori {-1, -1}
       */

      exit(0);
    }
    pthread_t threads[RICHIESTE * CLIENT];
    pthread_create(&threads[k], NULL, Prodotto, (void *)from_client);
    k++;
    /* TBD: Avviare un thread figlio per l'elaborazione del messaggio,
                    passandogli una **copia sullo heap** del messaggio ricevuto.
                    (ogni thread figlio deve elaborare un messaggio diverso)
     */
  }
}

void *Prodotto(void *v) {

  int ret;
  client_to_server *c = (client_to_server *)v;
  int v3 = (c->a) * (c->b); /* TBD: Calcolare il prodotto */
  server_to_client s;
  s.pid = c->pid;
  s.result = v3;
  /* TBD: Inviare il messaggio di risposta al client.
          Si chiami la funzione msgsnd() all'interno di una
                  sezione critica.
   */

  printf("\nSono Prodotto di Server. Invio del calcolo: %d\n\n", v3);

  ret = msgsnd(id_coda_risposte, &s, sizeof(server_to_client) - sizeof(long),
               s.pid); /* TBD */

  if (ret < 0) {
    perror("Errore invio risposta server");
    exit(1);
  }

  pthread_exit(NULL);
}
