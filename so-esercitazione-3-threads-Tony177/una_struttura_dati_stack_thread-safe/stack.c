#include "stack.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
void StackInit(Stack * s, int dim) {

	pthread_mutex_init(&(s->mutex), NULL);
	pthread_cond_init(&(s->cv_prod), NULL);
	pthread_cond_init(&(s->cv_cons), NULL);

	s->dati = (Elem *) malloc(sizeof(Elem)*dim);
	s->dim = dim;
	s->testa = 0;
}


void StackRemove(Stack * s) {

	pthread_mutex_destroy(&(s->mutex));
	pthread_cond_destroy(&(s->cv_prod));
	pthread_cond_destroy(&(s->cv_cons));

	free(s->dati);
}

void StackPush(Stack * s, Elem e) {

	pthread_mutex_lock(&(s->mutex));

	while(s->testa == s->dim)
		pthread_cond_wait(&(s->cv_prod), &(s->mutex));

	s->dati[s->testa] = e;
	s->testa++;
		printf("Inserimento: %d\n", e);
	pthread_cond_signal(&(s->cv_cons));
	pthread_mutex_unlock(&(s->mutex));

}


Elem StackPop(Stack * s) {

	int elemento;
	pthread_mutex_lock(&(s->mutex));

	while(s->testa == 0)
		pthread_cond_wait(&(s->cv_cons), &(s->mutex));

	s->testa--;	
	elemento=s->dati[s->testa];
	printf("Prelievo: %d\n", elemento);

	pthread_cond_signal(&(s->cv_prod));
	pthread_mutex_unlock(&(s->mutex));
	return elemento;
}
int StackSize(Stack * s) {
    
    int size;

    pthread_mutex_lock(&(s->mutex));

    size = s->testa;

    pthread_mutex_unlock(&(s->mutex));

    return size;
}
