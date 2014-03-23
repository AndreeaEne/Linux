/*
	Server TCP iterativ.
	Asteapta un mesaj de la clienti ce va fi trimis inapoi.
*/


#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* portul folosit */
#define PORT 8081

/* codul de eroare returnat de anumite apeluri */	
extern int errno;

int main()
{
	/* structurile folosite de server si client */
	struct sockaddr_in server;
	struct sockaddr_in from;
	char buffer[100];   /* mesajul trimis de client */
	int sd;				/* descriptorul de socket */


	/* cream un socket */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Eroare la socket().\n");
		return errno;
	}

	/* pregatim structurile de date */
	bzero (&server, sizeof (server));
	bzero (&from, sizeof (from));

	/* umplem structura folosita de server */
	server.sin_family = AF_INET; 					/* stabilirea familiei de socket-uri */
	server.sin_addr.s_addr = htonl (INADDR_ANY);	/* acceptam orice adresa */
	server.sin_port = htons (PORT);					/* utilizam un port utilizator */

	/* atasam socket-ul */
	if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
	{
		perror ("Eroare la bind().\n");
		return errno;
	}

	/* punem serverul sa asculte daca vin clientii sa se conecteze */
	if (listen (sd, 5) == -1)
	{
		perror ("Eroare la listen().\n");
		return errno;
	}

	char z[] = {'1','2','3','4'};

	srand (time(0); 		    /* generam un numar aleator intreg */
	for (int i = 0; i < 4; i++)
	{
		z[i] = rand() % 10;    /* ce va avea valori intre 0 si 9 */
	}


	/* Servim in mod iterativ cleintii */
	while(1)
	{
		int client;
		int length = sizeof(from);

		printf("Asteptam la portul %d...\n",PORT);
		fflush(stdout);

		/* Acceptam un client. Ne vom bloca pana la realizarea conexiunii */
		client = accept(sd,(struct sockaddr *) &from, &length);

		/* Eroare la acceptarea conexiunii */ 
		if(client < 0)
		{
			perror("Eroare la accept().\n");
			continue;
		}

		/* Am realizat conexiunea, astept maesajul...*/
		bzero (buffer,100);
		printf ("Asteptam mesajul...\n");
		fflush (stdout);

		/* Citirea mesajului */
		if (read(client, buffer, 100) <= 0)
		{
			perror("Eroare la read() de la client.\n");
			close(client);								/* Inchidem conexiunea cu clientul */
			continue;									/* Continuam sa ascultam */
		}

		printf("Mesajul a fost receptionat...\n Trimitem mesajul inapoi...");

		/* Regulile jocului */
		int x, h = 0;
		char buffer2[4];

		for (int i = 0; i < 4; i++)
		{
			printf("%d", z[i]);

			/* Verifica daca cifra este corecta si afiseaza un 0 */
			if( buffer[i] == z[i])
			{
				buffer2[h] = '0';
				h++; 
			}
		}

			/* Verifica faca cifra este corecta si se afla pe o pozitie diferita. Afiseaza un X */
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (z[j] == buffer[i] && i != j)
				{
					buffer2[h] = 'X';
					h++;
				}
			}

		}
		
		for (i = 0; i < 4; i++)
			buffer[i] = buffer2[i];
		for (i = 0; i < 4; i++)
			bufferuffer2[i] = ' ';


		/* Returnam mesajul inapoi */
		if (write(client, buffer, 100) <= 0)
		{
			perror("Eroare la write() catre client.\n");
			continue;	/* Continuam sa ascultam */
		}
			else 
			printf("transmitere cu succes.\n");

		/* Inchidem conexiunea */
		close(client);
	}
}
