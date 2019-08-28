struct param{
	int numero;
	char nick[256];
	char mensagem[256];
	int idCliente;
};

program PROG {
	version VERS {
		param ENVIAMSG(int) = 1;
		void RECEBEMSG(param) = 2;
	} = 1;
} = 0x31415926;
