#include "header.h"
#include <pthread.h>
#include <stdlib.h>


void inizializza(struct monitor* m){

	m->stazione=0;
	m->id_treno=0;

	m->num_lettori = 0;
	m->in_uso= 0;

	pthread_cond_init(&(m->cv_scrittori), NULL);
	pthread_cond_init(&(m->cv_lettori), NULL);

	m->coda_lettori = 0;
	m->coda_scrittori = 0;
	pthread_mutex_init(&(m->mutex), NULL);
}

void rimuovi (struct monitor* m){

	pthread_cond_destroy(&(m->cv_lettori));
	pthread_cond_destroy(&(m->cv_scrittori));
	pthread_mutex_destroy(&(m->mutex));
}


//SCRITTURA. AGGIORNAMENTO DELLA POSIZIONE DEL TRENO
void scrivi_stazione(struct monitor* m, int stazione){

	// SCRITTURA
	pthread_mutex_lock(&(m->mutex));
	while(m->in_uso== 1){
		m->coda_scrittori++;
		pthread_cond_wait(&(m->cv_scrittori),&(m->mutex) );
		m->coda_scrittori--;
	}
	m->in_uso=1;
	pthread_mutex_unlock(&(m->mutex));

	//SCRITTURA CON STARVATION DEGLI SCRITTORI
	m->stazione=stazione;

	pthread_mutex_lock(&(m->mutex));

	if(m->coda_scrittori >0)
		pthread_cond_signal(&(m->cv_scrittori));
	else if (m->coda_lettori >0)
		pthread_cond_broadcast(&(m->cv_lettori));

	m->in_uso=0;

	pthread_mutex_unlock(&(m->mutex));

}


//LETTURA. RESTITUISCE LA POSIZIONE DEL TRENO
int leggi_stazione(struct monitor* m){

	pthread_mutex_lock(&(m->mutex));
	while(m->num_lettori == 0 && m->in_uso == 1){
		m->coda_lettori++;
		pthread_cond_wait(&(m->cv_lettori), &(m->mutex));
		m->coda_lettori--;
	}
	m->in_uso=1;
	m->num_lettori++;
	pthread_mutex_unlock(&(m->mutex));

	// LETTURA CON STARVATION DEI LETTORI
	int ris=m->stazione;

	pthread_mutex_lock(&(m->mutex));
	m->num_lettori--;
	if(m->num_lettori == 0)
		{
		m->in_uso=0;
		pthread_cond_signal(&(m->cv_scrittori));
		}

	pthread_mutex_unlock(&(m->mutex));
	return ris;
}

