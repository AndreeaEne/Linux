#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

//portul folosit
#define PORT 8081

//eroarea returnata la unele apeluri
extern int errno;

//functie de convertire a adresei IP a clientului in sir de caractere
char *conv_addr(struct sockaddr_in address)
{
	static char str[25];
	char port[7];
	
	//adresa IP a clientului
	strcpy(str, inet_ntoa (address.sin_addr));
	//portul utilizat de client
	bzero(port,7);
	sprintf(port,":%d",ntohs(address.sin_port));
	strcat(str,port);
	return(str);
}

//the prog
int main()
{
	//structurile pt server si clienti
	struct sockaddr_in server;
	struct sockaddr_in from;

	//multimea descriptorilor de citire
	fd_set readfds;
	fd_set actfds;

	//structura de timp pt select()
	struct timeval tv;

	//descriptori de socket
	int sd, client;

	//descriptor folosit pt parcurgerea listelor de descriptori
	int fd;

	//maximul valorilor descriptorilor
	int nfds;

	//lungimea structurii sockaddr_in
	int len;

	//creare socket
	if((sd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("Eroare la socket()\n");
		return errno;
	}
	//pregatim structurile de date
	bzero(&server, sizeof(server));

	//umplem structura folosita de server
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl (INADDR_ANY);
	server.sin_port = htons (PORT);

	//atasam socketul
	if(bind(sd,(struct sockaddr *) &server, sizeof(struct sockaddr)) == -1)
	{
		perror("Eroare la bind().\n");
		return errno;
	}

	//punem serverul sa asculte daca vin clienti sa se conecteze
	if(listen(sd,5) == -1)
	{
		perror("Eroare la listen().\n");
		return errno;
	}

	//completam multimea de descriptori de citire
	//initial, multimea este vida
	FD_ZERO(&actfds);

	//includem in multime socketul creat
	FD_SET(sd,&actfds);

	//se va astepta un timp nedefinit
	tv.tv_sec=0;
	tv.tv_usec=0;

	//valoarea maxima a descriptorilor folositi
	nfds = sd;
	printf("Asteptam la portul %d...\n",PORT);
	fflush(stdout);

	//servim in mod concurent clientii...
	while(1)
	{
		//ajustam multimea descriptorilor activi(efectiv utilizati)
		bcopy((char *) &actfds, (char *) &readfds, sizeof(readfds));
		//apelul select()
		if(select(nfds+1, &readfds,NULL, NULL, &tv)<0)
		{
			perror("Eroare la select().\n");
			return errno;
		}
		//vedem daca e pregatit socketul pt a-i accepta pe clienti
		if(FD_ISSET(sd,&readfds))
		{
			//pregatirea structurii client
			len = sizeof(from);
			bzero(&from,sizeof(from));
			//a venit un client, acceptam conexiunea
			client = accept(sd,(struct sockaddr *) &from, &len);
			//eroare la acceptarea conexiunii de la un client
			if(client < 0)
			{
				perror("Eroare la accept().\n");
				continue;
			}
			//ajusteaza valoarea maximului
			if(nfds < client)
			  nfds = client;
			//includem in lista de descriptori activi si acest socket
			FD_SET(client, &actfds);
			printf("S-a conectat clientul cu descriptorul %d, de la adresa %s.\n",client, conv_addr(from));
			fflush(stdout);
		}
		//vedem daca e pregatit vreun socket client pt a face echo
		//parcurgem multimea de descriptori
		for(fd = 0;fd <= nfds;fd++)
		{
			//este un socket de citire pregatit?
			if(fd != sd && FD_ISSET(fd,&readfds))
			{
				//n-a putut fi trimis mesajul
				if(echo(fd) == 0)
				{
					printf("S-a deconectat clientul cu descriptorul %d.\n",fd);
					fflush(stdout);
					//inchidem conexiunea cu clientul
					close(fd);
					//scoatem si din multime
					FD_CLR(fd, &actfds);
				}
			}//if
		}//for
	}//while
}//main
//realizeaza primirea si retrimiterea unui mesaj unui client
int echo(int fd)
{
	//mesajul
	char buffer[100];
	//numarul de octeti cititi/scrisi
	int bytes;
	bytes = read(fd, buffer, sizeof(buffer));
	if(bytes < 0)
	{
		perror("Eroare la read() de la client.\n");
		return 0;
	}

	/* Regulile de generare a numarului */

	char z[] = {'1','2','3','4'};

	//srand((unsigned)time(0)); 		       /* generam 4 numere aleatoare intregi */
	/*for (int i = 0; i < 4; i++)
	{
		z[i] = rand() % 10;    /* ce vor avea valori intre 0 si 9 */
		//printf("%d\n", z[i]); - TEST - }

		int x, h = 0, i, j;
		char buffer2[4];

		printf("\n(Numarul este: ");
		for (int i = 0; i < 4; i++)
		{
			printf("%d", z[i] - 48);
		}
		printf(")\n");

			/* Verifica daca cifra este corecta si se afla pe o pozitie diferita. Afiseaza un X */
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if ((buffer[i] == z[j]) && (i != j))
				{
					buffer2[h] = 'X';
					h++;
				}
				else if ((buffer[i] == z[j]) && (i == j))
				{
					buffer2[h] = 'O';
					h++;
				}
			}

		}

		/* Copiem in buffer numarul */
		for (i = 0; i < 4; i++)
			buffer[i] = buffer2[i];
		for (i = 0; i < 4; i++)
			buffer2[i] = ' ';
	


	if(bytes && write(fd, buffer, bytes)<0)
	{
		perror("Eroare la write() catre client.\n");
		return 0;
	}
	return bytes;
}
