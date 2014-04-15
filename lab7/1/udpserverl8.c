#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define PORT 8081
//codul de eroare returnat de anumite apeluri
extern int errno;

char *intrebare(char *nume)
{

  FILE *fp;
  char *msg, msgAux[300];
  if((msg = malloc( sizeof(char) * 400)) == NULL)
  {
    printf("Nu pot aloca spatiu RAM pt. mesaj. Ies...\n\n");
    exit(1);
  }
  //citire din fisier
  if((fp = fopen(nume, "r")) == NULL)
  {
    printf("Nu pot deschide fisierul.Ies...\n\n");
    exit(1);
  }
  
  while(!feof(fp))
  {
    fgets(msgAux, 300, fp);
    strcat(msg, msgAux);
  }
  
  fclose(fp);
  return msg;
}


//the prog
int main()
{
	//structurile fol de server si client
	struct sockaddr_in adresa;
	struct sockaddr client;
	char buffer[400];
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
	if((sd=socket(AF_INET, SOCK_DGRAM,0))==-1)
	{
		perror("Eroare la socket().\n");
		return errno;
	}
	//pregatim structura folosita de server
	adresa.sin_family=AF_INET;
	//stabilirea fam de socket-uri
	adresa.sin_addr.s_addr=htonl(INADDR_ANY);
	//acceptam orice adresa
	adresa.sin_port=htons(PORT);
	//utilizam un port utilizator
	//atasam socketul
	if(bind(sd,(struct sockaddr *) &adresa, sizeof(struct sockaddr))==-1)
	{
		perror("Eroare la bind().\n");
		return errno;
	}

	int x=0;
	int i;
	int z;
	char raspuns[5]="dacda";
	//servim in mod iterativ clientii...
	for(i=0;i<5;i++)
	{

		z = rand()%5+1;
		int bytes;
		int length=sizeof(client);
		char fisier[10];
		snprintf(fisier, sizeof(fisier), "%d", z+1);
		strcat(fisier, ".txt");
		char *ask;
		char mclient[300];
		ask = intrebare(fisier);
		//citim mesajul de la client
		if((bytes=recvfrom(sd,mclient,300,0,&client,&length))<0)
		{
			perror("Eroare la recvfrom() de la client\n");
			return errno;
		}
		if(x==1)
		{
			if(strchr(mclient,raspuns[z-1]))
			{
				strcpy(buffer,"\n\nRaspuns corect\n");
				strcat(buffer, ask);
			}
			else
			{
				strcpy(buffer,"\n\nRaspuns gresit, ne pare rau\n");
			}
		}
		else
			{
				strcpy(buffer,ask);
				x=1;
			}
		
		//.... si il trimitem inapoi
		if(sendto(sd, buffer, strlen(buffer)+1, 0, &client, length)<0)
		{
			perror("Eroare la sendto() spre client.\n");
			return errno;
		}
		
	}

	
}
