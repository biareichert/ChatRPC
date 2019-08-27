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


/*
   Cliente -- Aciona funções do servidor
   Envia e recebe mensagens

*/

int main (int argc, char *argv[]) {
	//Variaveis compartilhadas
	int * queroEnviar = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	int * queroReceber = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	FILE *filewrite, *fileread;

	//Habilita o primeiro processo que fica verificando se
	//existem novas mensagens
	*queroReceber = 1;

	//Nenhuma mensagem para enviar então o processo começa
	//desabilitado
	*queroEnviar = 0;
	CLIENT *cl;
	//Quantidade mensagens recebidas até o momento
	int numeroMensagem = 0;
	char nome[20];
	mensa *nm;
	mensa bloco;
	char nick[256];
	char mensagem[50]; //nome do arquivo
	char text[256]; //linha do arquivo
	char texto[10000] = ""; //leitura do arquivo
	char nomeArquivo[20];

	//Verifica se IP está configurado
	if (argc != 2) {
		printf("Atencao: Coloque o IP do servidor\n");
		exit(1);
	}

	cl = clnt_create(argv[1], HELLO_WORLD_PROG, HELLO_WOLRD_VERS, "tcp");
	if (cl == NULL) {
		clnt_pcreateerror(argv[1]);
		exit(1);
	}

	//Obtem nick
	printf("Coloque um nick:\n");
	fgets(nick, sizeof(nick), stdin);
	strtok(nick, "\n");
	printf("Oi %s\n", nick);
	strcpy(bloco.nick, nick);

	//Fork necessario para que um processo receba mensagens
	// e o outro envie
    pid_t pid;

    if ((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }
		//printf("passei aqui\n");

    if (pid == 0){
				printf("Informe o nome do arquivo:\n");
				//printf("entrei aqui\n");
        //Lê mensagens do teclado e envia para o servidor
				while(1){
					//leitura do arquivo
					scanf("%s", mensagem);

					if(strcmp(mensagem, "/quit") == 0){
						//printf("oi");
						exit(1);
					}

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
					//printf("%s\n",texto);

					*queroEnviar = 1;
	    		while(*queroReceber == 1){
	    			//Ajuste técnico adaptativo para esperar o cliente
	    			//enviar mensagem
	    		}

	    		strcpy(bloco.mensagem, texto);
					recebemsg_1(&bloco,cl);
					*queroEnviar = 0;

	    	}
    }else{
				strcpy(nomeArquivo,nick);
				strcat(nomeArquivo,".client");
        //Verifica novas mensagens no servidor
				while(1){

					nm = num_1(&numeroMensagem, cl);


					if (nm == NULL) {
						clnt_perror(cl,argv[1]);
						exit(1);
					}
				//	printf("%d\n",nm->numero);
					if(nm->numero != -1){

						printf("<%s> %s\n", nm->nick, nm->mensagem);
						numeroMensagem = nm->numero;

					}
					printf("%s", nm->nick);

					filewrite = fopen(nomeArquivo,"w");
					int i;
					for(i=0; nm->mensagem[i]; i++){
						fputc(nm->mensagem[i],filewrite);
					}

					fclose(filewrite);

					if(*queroEnviar == 1){
						*queroReceber = 0;
						while(*queroEnviar == 1){
							//Ajuste técnico adaptativo para esperar o cliente
	    				//enviar mensagem
						}
						*queroReceber = 1;
					}

				}
    }
	return 0;
}
