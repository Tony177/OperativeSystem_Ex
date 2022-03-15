#include "stack.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void *Inserisci(void * s)
{

	int i;
	Elem v;


	for(i=0; i<4; i++) {

		v = rand() % 11;
		
		StackPush((Stack *)s,v);
	

		sleep(1);
	}

	pthread_exit(NULL);
}


void *Preleva(void * s)
{

	int i;
	Elem v1, v2;

    
	for(i=0; i<10; i++) {

		v1 = StackPop((Stack *)s);

		v2 = StackPop((Stack *)s);
		/* TBD: Prelevare con StackPop() in v2 */

		printf("Somma: %d\n", v1+v2);

		sleep(3);
	}

	pthread_exit(NULL);

}


int main(int argc, char *argv[])
{

	int rc;
	int i;
	pthread_t thread_prod[i];
	pthread_t thread_cons;

	srand(time(NULL));


	Stack * stack = malloc(sizeof(Stack));
	StackInit(stack,4);



	for(i=0; i<5; i++) {

		pthread_create(&thread_prod[i], NULL, Inserisci, stack);
	}

	pthread_create(&thread_cons, NULL, Preleva, stack);

	for(i=0; i<5; i++) {
		pthread_join(thread_prod[i], NULL);
		/* TBD: Effettuare la join con i thread figli */
	}
	pthread_join(thread_cons, NULL);


	StackRemove(stack);
	/* TBD: Disattivare l'oggetto stack con StackRemove() */
}

