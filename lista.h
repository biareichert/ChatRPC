#ifndef LISTA_HEADER
#define LISTA_HEADER

#include <stdio.h>
#include <stdlib.h>

typedef struct no {
  int numero;
  char nick[256];
  char mensagem[256];
  struct no * proximo;
} No;

typedef struct lista {
  No * cabeca; 
} Lista;

Lista * listavazia();
void adicionar(int numero, char *nick, char *mensagem, Lista * lista);
void destroi(Lista * lista);

#endif

