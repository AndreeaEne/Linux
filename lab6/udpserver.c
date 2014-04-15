#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

# define PORT 8081

/* Codul de eroare returnat de anumite apeluri */
extern int errno;


int main()
{
	/* Structurile folosite de server si client */
	struct sockaddr_in adresa;
	struct sockaddr client;
	char buffer[100];
	int sd;

	//lansam sv in fundal...
	switch(fork())
	{
		case -1:
			perror("Fork error\n");
			return errno;
		case 0:
			break;
		default:
			printf("Serverul a fost lansat in fundal...\n");
			exit(0);
	}
	//creem un socket
	if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("Eroare la socket().\n");
		return errno;
	}

	/* Pregatim structura folosita de server */
	adresa.sin_family=AF_INET;

	//*Stabilirea fam de socket-uri */
	adresa.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Acceptam orice adresa */
	adresa.sin_port = htons(PORT);

	/* Utilizam un port utilizator si atasam socket-ul */
	if(bind(sd, (struct sockaddr *) &adresa, sizeof(struct sockaddr)) == -1)
	{
		perror("Eroare la bind().\n");
		return errno;
	}
		
	int var;

	srand((unsigned)time(0)); 	 /* generam 1 numar aleator intreg */
	var = rand() % 4 + 1;    /* ce va avea valori intre 0 si nrMax_intrebari */
		//z[i] = 48 + var;
/*
	char q[30];
	strcat(q, "q");
	strcat(q, var);
	strcat(q, ".txt");
	
	puts(q);
*/
	File *fp;
	fp = fopen("q1.txt", "r");	

	//servim in mod iterativ clientii...
	while ((fgetc("q1.txt")) != EOF)
	{
		int bytes;
		int length = sizeof(client);
		//citim mesajul de la client
		if((bytes = recvfrom(sd, buffer, 100, 0, &client, &length)) < 0)
		{
			perror("Eroare la recvfrom() de la client\n");
			return errno;
		}

		/* Regulile de generare a numarului 
		int x, h = 0, i, j;
		char buffer2[4];


		printf("\n(Intrebarea este: ");
		for (i = 0; i < 3; i++)
		{
			printf("%d", z[i] - 48);

			/* Verifica daca cifra este corecta si afiseaza un 0 
		}
		printf(")\n");
		*/

		}

		/* Copiem in buffer numarul */
		for (i = 0; i < 4; i++)
			buffer[i] = buffer2[i];
		for (i = 0; i < 4; i++)
			buffer2[i] = ' ';
		
		//.... si il trimitem inapoi
		if(sendto(sd, buffer, bytes, 0, &client, length) < 0)
		{
			perror("Eroare la sendto() spre client.\n");
			return errno;
		}
	}
}