Grafo delle dipendenze
======================

Si realizzi in linguaggio C/C++ un'applicazione **multiprocesso** per il
calcolo distribuito della formula

        y = ( a*b + (c+d)*(e+f) + g*h ) .

Il calcolo deve essere suddiviso tra più processi sulla base del **grafo
delle dipendenze** riportato sotto. I processi si scambiano operandi e
risultati utilizzando **code di messaggi UNIX**. Ciascun processo si
mette in attesa di ricevere gli operandi di cui ha bisogno da una coda,
e ritorna il risultato al processo richiedente attraverso un'ulteriore
coda. Il processo P1 genera un vettore casuale di operandi, e richiede
delle elaborazioni, da svolgere **in parallelo**, ai processi P2 (sugli
operandi **a** e **b**), P3 (operandi **c**, **d**, **e**, **f**) e P4
(operandi **g** e **h**). Il processo P3 a sua volta manda delle
richieste di elaborazione, anche esse in parallelo, ai processi P5
(operandi c e d) e P6 (operandi e e f), e ne attende i risultati prima
di produrre un risultato a sua volta. Il processo P1 deve stampare a
video gli operandi che esso genera, i risultati che riceve e il valore
di y ottenuto. Il processo P1 ripete l'intero procedimento per 3 volte.

Si richiede che lo studente sviluppi **6 programmi separati, uno
per ogni processo P1\...P6, e che vengano generati 6 eseguibili dalla
compilazione**. Un ulteriore programma deve generare i processi
P1\...P6, tramite l'uso delle primitive `fork()` ed `exec()`, ed
attenderne la terminazione.

Utilizzare 4 code di messaggi come in figura. I processi P1, P2, P3 e P4
usano la prima coppia di code. Per fare in modo che i processi P2, P3 e P4
ricevano gli operandi ad essi destinati (es., **a** e **b** devono essere
ricevuti da P2), il processo P1 deve indicare il destinatario nel campo **tipo**
del messaggio con gli operandi (es., scrivendo il valore `2').
Nei messaggi di risposta, i processi possono indicare P1 nel campo **tipo**.
I processi P3, P5 e P6 usano la seconda coppia di code, analogamente.

**Suggerimento:** I messaggi che sono inviati su una stessa coda devono avere
tutti la stessa dimensione. Per ogni coda, definire una `struct' differente per
i messaggi da inviare su quella coda. Ad esempio, si definisca una `struct` per
i messaggi per la coda degli operandi \#1, includendo 4 variabili di tipo intero. 
Per gli operandi destinati a P3, si utilizzeranno tutti le 4 variabili. Per gli
operandi destinati a P2 e P4, si utilizzeranno solo 2 delle 4 variabili. 

File da modificare:
- start.c
- p.h
- p1.c
- p2.c
- p3.c
- p4.c
- p5.c
- p6.c


![image](https://github.com/rnatella/esercizi_linux/blob/master/images/ambiente_locale/code_messaggi/grafo_delle_dipendenze_code.png)
