
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "server.h"

struct sockaddr_in caddr;
struct sockaddr_in saddr;
int client, server, csize;
int srecv;
char *buf;

// SOCK_STREAM
// SOCK_D
int main() {

	int stop = 0;
	char recvbuf[BUFLEN];
        int recvbuflen = BUFLEN;

	buf = (char*) malloc(MEM_LEN);
        memset(buf, 0, MEM_LEN);

        setlocale(LC_ALL, "portuguese");
	printf("\t-> Ligando o servidor...\n");

        saddr.sin_family        = AF_INET;
        saddr.sin_addr.s_addr   = htonl(INADDR_ANY);
        //saddr.sin_addr.s_addr = inet_addr("105.168.57.125");
        saddr.sin_port  = htons(SERVER_PORT);

        csize = sizeof(struct sockaddr_in);
        server = socket(AF_INET, SOCK_STREAM, 0/*IPPROTO_TCP*/);
	//TODO verificar erro

	printf("\t[+] Socket criado com sucesso!\n");

	printf("\t-> bind() ...\n");
        int r = bind(server, (struct sockaddr*)&saddr, sizeof(struct sockaddr_in));
        if(r)
        {
		printf("\t[-] bind() falhou! Erro: %d\n", r);
                close(server);
                exit(1);
        }

	printf("\t[+] Sucesso!\n"); 

        listen(server, 8);
	//TODO verificar erro

    	do{
		printf("\t-> Aguardando conexoes, accept()...\n");
                client = accept(server, (struct sockaddr*)&caddr, &csize);
		//TODO verificar erro
		printf("\t[+] sucesso\n");


//		do{
		memset(recvbuf, 0, 0x1000);
        	printf("\t-> Aguardando dados...\n");
                srecv = recv(client, recvbuf, recvbuflen, 0);
                int z  = srecv;
		if(srecv){
			if(!strcmp("sair",recvbuf) ){
                                close(client);
                                break;
                        }


                       	printf("\t[+] Bytes recebidos: %d\n", srecv);
			putchar('\n');
                	srecv = get(buf, recvbuf, srecv);
			if( srecv < 1)
			{
				printf("\t[-] GET HTTP /1.1 falhou!\n\n");
				//enviar resposta de erro
				send(client, recvbuf, z, 0);
				close(client);
				continue;
			}

			printf("\t-> Enviando dados...\n");
			srecv = send(client, buf, srecv, 0);
                	if( srecv == -1 ){
                        	printf("\t[-] send() falhou! Erro: %d\n\n", srecv);
                        	close(client);
                		continue;
			}

			printf("\t[+] Sucesso! Bytes enviados: %d\n", srecv);
			close(client);
                }else if( srecv == 0)
		{
                	printf("\t[-] Conexao fexada pelo cliente\n");
			close(client);
        	} else {
                	printf("\t[-] recv() falhou! Erro: %d\n\n", srecv);
                	close(client);
                	continue;
        	}

	}while(1);

	printf("\t-> Encerrando o servidor...\n");
	//shutdown()

	printf("\t[+] Encerrado!\n");

	free(buf);
	close(server);
}
