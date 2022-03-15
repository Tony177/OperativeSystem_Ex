#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "coda_richieste.h"

#define DIM_DISCO 20

void Schedulatore(coda_richieste *c) {

  int disco[DIM_DISCO];
  int posizione_corrente = 0;
  richiesta r;

  for (int i = 0; i < 25; i++) {
    preleva_richiesta(c, &r);
    /* TBD: Effettuare la consumazione */

    printf("[%d] Prelevo richiesta: posizione=%d, processo=%d\n", getpid(),
           r.posizione, r.processo);

    int attesa = abs(posizione_corrente - r.posizione);

    printf("[%d] Attesa Schedulatore... (%d secondi)\n", getpid(), attesa);

    sleep(attesa);

    disco[r.posizione] = r.processo;

    posizione_corrente = r.posizione;
  }
}

void Utente(coda_richieste *c) {
  richiesta r;
  for (int i = 0; i < 5; i++) {
    r.posizione = rand() % 19;
    r.processo = getpid();
    inserisci_richiesta(c, &r);
    printf("[%d] Richiesta Utente: posizione=%d, processo=%d\n", getpid(),
           r.posizione, r.processo);

    /* TBD: Effettuare la produzione */
  }
}

int main() {
  coda_richieste *c = inizializza_coda();
  /* TBD: Allocazione coda richieste, chiamando "inizializza_coda()" */
  pid_t pid = fork();
  if (pid == 0) {
    Schedulatore(c);
    exit(0);
  } else if (pid < 0) {
    perror("Errore creazione schedulatore");
    exit(1);
  }
  for (int i = 0; i < 5; i++) {
    pid = fork();
    if (pid == 0) {
      Utente(c);
      exit(0);
    } else if (pid < 0) {
      perror("Errore creazione schedulatore");
      exit(1);
    }
  }
  /* TBD: Creazione del processo Schedulatore */
  /* TBD: Creazione dei processi Utente */
  for (int i = 0; i < 6; i++)
    wait(NULL);
  printf("Rimozione code...\n");
  rimuovi_coda(c);
  printf("Conlcusione programma...\n");
  /* TBD: Attesa terminazione processi figli */
}