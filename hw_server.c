#include <rpc/rpc.h>
#include "hw.h"
#include "lista.c"

//Quantidade de mensagens atualmente no servidor
int numMensagem = 0;

//Lista onde são armazenadas as mensagens
Lista * lista;

//Recebe o identificador da ultima mensagem e verifica se
//existem mensagens novas
param *enviamsg_1_svc(int *numMSG,struct svc_req *req){
	static param *mensagemBloco;
	static param semMsg;

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

//Recebe mensagens dos clientes, coloca em uma lista e grava um novo arquivo.serv
void *recebemsg_1_svc(param *bloco, struct svc_req *req){
	FILE *filewrite;
	int i;
	printf("ola");

	char nick[256];
	char mensagem[256];
	char nomeArquivo[20];
	int num;

	strcpy(nick, bloco->nick);
	strcpy(mensagem, bloco->mensagem);
	strcpy(nomeArquivo, nick);
	num = bloco->idCliente;
	//strcat(nomeArquivo, ".serv");

	//Inicializa lista
	if(lista == NULL){
		lista = listavazia();
	}

	numMensagem++;

	//Adiciona na lista
	adicionar(numMensagem, nick, mensagem, lista);

	//gravando arquivo .serv
	sprintf (nomeArquivo, "%s-0%d.serv",nick, num);
	filewrite = fopen(nomeArquivo,"w");
	for(i=0; mensagem[i]; i++){
		fputc(mensagem[i],filewrite);
	}

	fclose(filewrite);
}
