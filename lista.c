#include "lista.h"

//Cria no da lista
No * criano(int numero, char *nick, char *mensagem);

No * criano(int numero, char *nick, char *mensagem){
  No * novoNo = malloc(sizeof(No));
  novoNo->numero = numero;
  strcpy(novoNo->nick, nick);
  strcpy(novoNo->mensagem, mensagem);
  novoNo->proximo = NULL;
  return novoNo;
}
//Inicializa a lista
Lista * listavazia(){
  Lista * lista = malloc(sizeof(Lista));
  lista->cabeca = NULL;
  return lista;
}

//Procura por nova mensagem na lista e retorna uma struct com
//a mensagem, nick do remetente e número atual da mensagem
mensa * pegaMSG(Lista * lista, int numeroCli) {
   No * noAtual = lista->cabeca;
   mensa *bloco;
   int numero = numeroCli+1;
   if(lista->cabeca == NULL || numero == 0){
      bloco->numero = -1;
      return bloco;
   } 
   //Procura mensagem  
   while(noAtual->proximo != NULL && noAtual->numero < numero){
      noAtual = noAtual->proximo;
   }
   //Grava mensagem no bloco
   if(noAtual->numero == numero){
      bloco->numero = numero;
      strcpy(bloco->nick, noAtual->nick);
      strcpy(bloco->mensagem, noAtual->mensagem);
   }
   //Retorna o bloco com os dados gravados
   return bloco;
 }



//Adiciona um bloco com mensagem, nick e numero na lista
void adicionar(int numero, char *nick, char *mensagem, Lista * lista){
   No * noAtual = NULL;
   if(lista->cabeca == NULL){
     lista->cabeca = criano(numero, nick, mensagem);
   }
   else {
     noAtual = lista->cabeca; 
     while (noAtual->proximo!=NULL){
          noAtual = noAtual->proximo;
     }
     noAtual->proximo = criano(numero, nick, mensagem);
   }
}

//Destroi a lista
void destroi(Lista * lista){
  No * noAtual = lista->cabeca;
  No * proximo = noAtual;
  while(noAtual != NULL){
    proximo = noAtual->proximo;
    free(noAtual);
    noAtual = proximo;
  }
  free(lista);
}
