#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

#include "serversync.h"

void send_sinc(int ok_id, int req_id, messaggio *msg) {

  int ret;
  request_to_send req;
  req.type = getpid();

  printf("[%d] Client: invio request-to-send, type=%ld\n", getpid(), req.type);

  ret = msgsnd(req_id, &req, sizeof(request_to_send) - sizeof(long), 0);

  if (ret < 0) {
    perror("Errore msgsnd (request-to-send)");
    exit(1);
  }

  printf("[%d] Client: in attesa di ok-to-send...\n", getpid());
  ok_to_send ok;
  ret = msgrcv(ok_id, &ok, sizeof(ok_to_send) - sizeof(long), 0, 0);

  if (ret < 0) {
    perror("Errore msgrcv (ok-to-send)");
    exit(1);
  }

  printf("[%d] Client: ricevuto ok-to-send... type=%ld, id_coda=%d\n", getpid(),
         ok.type, ok.id_coda);

  int id_coda = ok.id_coda;

  printf("[%d] Client: invio messaggio, coda=%d, type=%ld, valore=%d\n",
         getpid(), id_coda, msg->type, msg->val);

  ret = msgsnd(id_coda, msg, sizeof(messaggio) - sizeof(long), 0);

  if (ret < 0) {
    perror("Errore msgsnd (coda messaggi)");
    exit(1);
  }
}

void receive_sinc(int msg_id, int ok_id, int req_id, messaggio *msg) {

  int ret;
  request_to_send req;

  printf("[%d] Server: in attesa di request-to-send...\n", getpid());

  ret = msgrcv(req_id, &req, sizeof(request_to_send) - sizeof(long), 0, 0);

  printf("[%d] Server: ricevuto request-to-send, type=%ld\n", getpid(), req.type);

  if (ret < 0) {
    perror("Errore msgrcv (request-to-send)");
    exit(1);
  }

  ok_to_send ok;
  ok.type = getpid();
  ok.id_coda = msg_id;

  printf("[%d] Server: invio ok-to-send, type=%ld, id_coda=%d\n", getpid(),
         ok.type, msg_id);

  ret = msgsnd(ok_id, &ok, sizeof(ok_to_send) - sizeof(long), 0);

  if (ret < 0) {
    perror("Errore msgsnd (ok-to-send)");
    exit(1);
  }

  printf("[%d] Server: in attesa del messaggio...\n", getpid());

  ret = msgrcv(msg_id, msg, sizeof(messaggio) - sizeof(long), 0, 0);

  if (ret < 0) {
    perror("Errore msgrcv (coda messaggi)");
    exit(1);
  }

  printf("[%d] Server: ricevuto messaggio, type=%ld, valore=%d\n", getpid(),
         msg->type, msg->val);
}
