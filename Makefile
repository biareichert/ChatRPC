all: client server

hw.h: hw.x
	rpcgen hw.x
	cc -c hw_xdr.c
hw_xdr.c hw_svc.c hw_clnt.c hw_cliente.c: hw.h

client: hw_cliente.o hw_clnt.o hw_xdr.o
	cc hw_cliente.c hw_clnt.c hw_xdr.o -o cliente -lnsl

server: hw_server.o hw_svc.o hw_xdr.o
	cc hw_svc.c hw_server.c hw_xdr.o -o server -lnsl

.PHONY: clean

clean:
	-rm *.o
	-rm client*
	-rm server*
	-rm hw.h
	-rm hw_clnt.c
	-rm hw_svc.c
	-rm hw_xdr.c
