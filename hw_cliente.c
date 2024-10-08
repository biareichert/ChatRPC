#include <sys/mman.h>
#include <stdio.h>
#include <rpc/rpc.h>
#include "hw.h"
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
	FILE *filewrite, *fileread;
	int stop = 0;
	int opc, n;
	CLIENT *cl;
	int numeroMensagem = 0; //Quantidade mensagens recebidas até o momento
	char nome[20];
	char sair[10];
	param *nm;
	param bloco;
	char nick[256];
	char mensagem[50]; //nome do arquivo
	char text[256]; //linha do arquivo
	char texto[10000] = ""; //leitura do arquivo
	char nomeArquivo[20];
	int id = 0;

	//Verifica se IP está configurado
	if (argc != 2) {
		printf("Atencao: Coloque o IP do servidor\n");
		exit(1);
	}

	cl = clnt_create(argv[1], PROG, VERS, "tcp");
	if (cl == NULL) {
		clnt_pcreateerror(argv[1]);
		exit(1);
	}

	//Obtem nick
	printf("Informe um nickname:\n");
	fgets(nick, sizeof(nick), stdin);
	strtok(nick, "\n");
	printf("Bem vindo %s!\n", nick);
	strcpy(bloco.nick, nick);
	bloco.idCliente = id++;

  while (stop != 2){
		printf("Deseja mandar um arquivo?\n");
		printf("0- Sim\n");
		printf("1- Apenas receber as mensagens\n");
		printf("2- Sair\n");
		scanf("%d",&opc);

		switch(opc){
			case 0:
				printf("Informe o nome do arquivo.\n");
				scanf("%s", mensagem);
				fileread = fopen(mensagem,"r");
				if(fileread == NULL){
					printf("Erro ao abrir o arquivo %s.\n", mensagem);
					break;
				}
				if(fileread){
					while((fgets(text, sizeof(text),fileread)) != NULL){
						strcat(texto,text);
					}
				}
				strcpy(bloco.mensagem, texto);
				recebemsg_1(&bloco,cl);
				break;
			case 1:
				strcpy(nomeArquivo,nick);

				while(1){

					nm = enviamsg_1(&numeroMensagem, cl);
					if (nm == NULL) {
						clnt_perror(cl,argv[1]);
						exit(1);
					}

					if(nm->numero != -1){
						printf("\n<%s> %s\n", nm->nick, nm->mensagem);
						numeroMensagem = nm->numero;
						sprintf (nomeArquivo, "%s0%d.client0%d",nick, nm->numero, bloco.idCliente);
						filewrite = fopen(nomeArquivo,"w");
						int i;
						for(i=0; nm->mensagem[i]; i++){
							fputc(nm->mensagem[i],filewrite);
						}
					}else{
						printf("\nSem mais mensagens.\n" );
						printf("0- Para voltar ao menu.\n");
						printf("1- Para continuar a receber mensagens.\n");
						scanf("%d",&n);
						if(n == 0){
							break;
						}else{
							continue;
						}
					}

			  }

				fclose(filewrite);

			  break;

			case 2:
				stop = 2;
				break;
		}
  }
	return 0;
}
