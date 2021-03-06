#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

void InizioLettura(Monitor *m, Buffer *buf) {

  /* TBD: Effettuare inizio lettura */
  enter_monitor(m);

  if (buf->num_lettori == 0) {
    wait_condition(m, buf->cv_lettori);
  }
  buf->num_lettori++;

  leave_monitor(m);
}

void FineLettura(Monitor *m, Buffer *buf) {

  enter_monitor(m);

  buf->num_lettori--;
  if (buf->num_lettori > 0) {
    signal_condition(m, buf->cv_lettori);
  } else
    signal_condition(m, buf->cv_scrittori);

  leave_monitor(m);
  /* TBD: Effettuare fine lettura */
}

// Procedure di inizio e fine scrittura

void InizioScrittura(Monitor *m, Buffer *buf) {

  enter_monitor(m);

  if (buf->num_lettori > 0) {
    wait_condition(m, buf->cv_scrittori);
  }

  leave_monitor(m);
  /* TBD: Effettuare inizio scrittura */
}

void FineScrittura(Monitor *m, Buffer *buf) {
  enter_monitor(m);

  signal_condition(m, buf->cv_lettori);

  leave_monitor(m);
  /* TBD: Effettuare inizio scrittura */
}

void Servizio(Monitor *m, Buffer *buf) {

  srand(time(0));

  int i;
  for (i = 0; i < 20; i++) {

    /* TBD: Richiamare InizioScrittura e FineScrittura */
    InizioScrittura(m, buf);
    buf->meteo.temperatura = (rand() % 101) - 50;
    buf->meteo.umidita = rand() % 101;
    buf->meteo.pioggia = rand() % 2;
    FineScrittura(m, buf);
    printf("<%d> scrittura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(),
           buf->meteo.temperatura, buf->meteo.umidita,
           (buf->meteo.pioggia ? "si" : "no"));

    sleep(2);
  }
}

void Utente(Monitor *m, Buffer *buf) {

  int i;
  for (i = 0; i < 10; i++) {

    /* TBD: Richiamare InizioLettura e FineLettura */
    InizioLettura(m, buf);
    printf("<%d> lettura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(),
           buf->meteo.temperatura, buf->meteo.umidita,
           (buf->meteo.pioggia ? "si" : "no"));
    FineLettura(m, buf);
    sleep(1);
  }
}
