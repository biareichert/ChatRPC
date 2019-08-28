struct mensa{
	int numero;
	char nick[256];
	char mensagem[256];
	int idCliente;
};

program HELLO_WORLD_PROG {
	version HELLO_WOLRD_VERS {
		mensa NUM(int) = 1;
		void RECEBEMSG(mensa) = 2;
	} = 1;
} = 0x31415926;
