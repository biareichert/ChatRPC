#include <rpc/rpc.h>
#include "hw.h"
#include "lista.c"

/*
   Servidor - Guarda as mensagens que vão chegando
   dos clientes e envia conforme a solicitação
   Aqui ficam as funções que são chamadas pelo cliente.
*/

//Quantidade de mensagens atualmente no servidor
int numMensagem = 0;

//Lista onde são armazenadas as mensagens
Lista * lista;

//Recebe o identificador da ultima mensagem e verifica se
//existem mensagens novas
mensa *num_1_svc(int *numMSG, struct svc_req *req){
	static mensa *mensagemBloco;
	static mensa semMsg;

	//Se não existem novas mensagens avisa o cliente
	if(numMensagem == 0 || *numMSG == numMensagem){
		semMsg.numero = -1;
		return &semMsg;
	}

	//Inicializa a lista
	if(lista == NULL){
		lista = listavazia();
	}

	//Com novas mensagens pega a mensagem mais antiga entre as novas
	if(*numMSG < numMensagem){
		mensagemBloco = pegaMSG(lista, *numMSG);
		return mensagemBloco;

	}
	return mensagemBloco;
}

//Recebe mensagens dos clientes e coloca numa lista
void *recebemsg_1_svc(mensa *bloco, struct svc_req *req){
	FILE *filewrite;
	int i;

	char nick[256];
	char mensagem[256];
	char nomeArquivo[20];

	strcpy(nick, bloco->nick);
	strcpy(mensagem, bloco->mensagem);
	strcpy(nomeArquivo, nick);
	strcat(nomeArquivo, ".serv");

	//Inicializa lista
	if(lista == NULL){
		lista = listavazia();
	}

	numMensagem++;
  //printf("%s:%s\n",bloco->nick, bloco->mensagem);
	//Adiciona na lista
	adicionar(numMensagem, nick, mensagem, lista);


	//gravando arquivo .servidor
	filewrite = fopen(nomeArquivo,"w");
	for(i=0; mensagem[i]; i++){
		fputc(mensagem[i],filewrite);
	}

	fclose(filewrite);

}
