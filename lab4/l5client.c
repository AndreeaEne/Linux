/*
	Client TCP.
	Trimite un mesaj unui server..
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* Codul de eroare returnat de anumite apeluri */	
extern int errno;

/* Portul de conectare la server */
int port;

int main(int argc, char *argv[])
{
	int sd;							/* Descriptorul de socket */
	int i,var;
	struct sockaddr_in server;		/* Structura folosita pentru conectare */
	char buffer[100];				/* Mesajul trimis */

	/* Verifica daca exista toate argumentele in linia de comanda */
	if(argc != 3)
	{
		printf("Sintaxa: %s <adresa_server> <port>\n",argv[0]);
		return -1;
	}

	while(strcmp(buffer,"0000"))
	{
		/* Stabilim portul */
		port = atoi(argv[2]);

		/* Cream socket-ul */
		if((sd = socket (AF_INET,SOCK_STREAM,0)) == -1)
		{
			perror("Eroare la socket().\n");
			return errno;
		}

		/* Umplem structura folosita pentru realizarea conexiunii cu serverul */
		server.sin_family = AF_INET;						/* Familia socket-ului */
		server.sin_addr.s_addr = inet_addr(argv[1]);		/* Adresa IP a server-ului in codificarea retea htonl() nu e necesar. inet_arrd furnizeaza rezultatul in codificarea retea */
		server.sin_port = htons(port);						/* Portul de conectare */

		/* Ne conectam la server */
		if(connect(sd,(struct sockaddr *) &server, sizeof(struct sockaddr)) == -1)
		{
			perror("Eroare la connect().\n");
			return errno;
		}

		/* Citirea mesajului si trimiterea catre server */
		bzero(buffer, 100);
		printf("Introduceti mesajul: ");
		fflush(stdout);
		read(0, buffer, 100);

		if(write(sd,buffer,100) <= 0)
		{
			perror("Eroare la write()) spre server.\n");
			return errno;
		}

		/* Citirea raspunsului de la server. Ne blocam pana cand serverul raspunde */
		if(read(sd,buffer,100) < 0)
		{
			perror("Eroare la read() de la server.\n");
			return errno;
		}

		/* Afisam rezultatul primit */
		printf("Mesajul primit este: %s.\n", buffer);
	}

	/* Inchidem conexiunea */
	close(sd);
}
