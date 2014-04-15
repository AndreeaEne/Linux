#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<string.h>

//codul de eroare returnat de anumite apeluri
extern int errno;
//portul de conectare la server
int port;
//prog

int main(int argc,char *argv[])
{
	//descriptor de socket
	int sd;
	//structura folosita pt conectare
	struct sockaddr_in server;
	//mesaj trimis
	char buffer[100];
	int length;
	//exista toate argumentele in linia de comanda?
	if(argc !=3)
	{
		printf("Sintaxa %s <adresa_server> <port>\n",argv[0]);
		return -1;
	}
	//stabilim portul
	while(1)
	{
		port=atoi(argv[2]);
		//creem socketul
		if((sd=socket(AF_INET,SOCK_DGRAM,0))==-1)
		{
			perror("Eroare la socket().\n");
			return errno;
		}
		
		//umplem structura folosita pt realizarea dialogului cu serverul
		server.sin_family=AF_INET;
		//familia socketului
		server.sin_addr.s_addr = inet_addr (argv[1]);
		//adresa ip a serverului
		server.sin_port = htons(port);
		//portul de conectare
		
		//citirea mesajului de la intrarea standard
		bzero(buffer,100);
		printf("Introduceti mesajul: ");
		fflush(stdout);
		read(0, buffer, 100);
		length = sizeof(server);
		//trimitem mesajul catre server
		if(sendto(sd,buffer,strlen(buffer),0,&server,length)<0)
		{
			perror("eroare la sendto() spre server.\n");
			return errno;
		}
		//citirea raspunsului dat de server(ne blocam pana raspunde)
		if(recvfrom(sd,buffer,100,0,&server,&length)<0)
		{
			perror("Eroare la recvfrom() de la server.\n");
			return errno;
		}
		printf("Mesajul primit este: '%s'.\n",buffer);
	}
		//inchidemm socketul, finish
	close(sd);

	return 0;
}