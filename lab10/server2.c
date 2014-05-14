/*Server TCP concurent (echo) */

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<unistd.h>
#include<error.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<signal.h>
#include<errno.h>
#include<stdio.h>


#define MAX_CLIENTS 10 /*numar maxim de clienti */
#define FULL "Nu mai pot fi acceptati alti clienti...\n"
#define MESSAGE "Mesajul: "
#define MES_LEN strlen(MESSAGE)

struct sockaddr_in address;
extern int errno;

/*******************structura*******************************/
class baza {
public:
  char nume[20];
  char prenume[40];
  int varsta;
  char localitate[30];
};

struct baza pers[10];


/*************************************************************/
/**********************Functie >30 ani************************/
void func1(char *nume)
{
  int i;
  for(i = 0; i < 4; i++)
    if(pers[i].varsta > 30)
    {
      strcat(nume, pers[i].nume);
      strcat(nume, " ");
      strcat(nume, pers[i].prenume);
      strcat(nume, " \n");
    }
    strcat(nume, "\n");
}


/**************************************************************/
/***************************Functie =Bucuresti**8**************/
void func2(char *nume)
{
  int i;
  for(i = 0; i < 4; i++)
    if(!strcmp(pers[i].localitate, "Bucuresti"))
    {
      strcat(nume, pers[i].nume);
      strcat(nume, " ");
      strcat(nume, pers[i].prenume);
      strcat(nume, " \n");
    }
    strcat(nume, "\n");
}
/***************************************************************/
/***************************Functie C___************************/
void func3(char *nume)
{
  int i;
  for(i = 0; i < 4; i++)
    if(pers[i].nume[0] == "C")
    {
      strcat(nume, pers[i].nume);
      strcat(nume, " ");
      strcat(nume, pers[i].prenume);
      strcat(nume, " \n");
    }
    strcat(nume, "\n");
}
/******************************************************************/


/**************************PORTUL*******************************
 **************************************************************/ 
int port = 8081;

int sd, cd, l;
char buffer[100];
int nr_cl = 0;
fd_set citire, scriere;
struct timeval tv;

char * ConvAddr()
{
  char *p;
  char nr[10];
  
  if ((p = (char *) malloc(25)) == NULL)
    return inet_ntoa(address.sin_addr);
  p = inet_ntoa(address.sin_addr); // converts from network address to dots-and-numbers address(ASCII)
  bzero(nr, 7); // puts 7 bytes of 0 in nr (&nr[0])
  sprintf(nr, ":%d", ntohs(address.sin_port)); //formatting like ipAddress:portNumber
  strcat(p, nr);
  return p; //returns the address formated
}

void tratare_client()
{
  char s = 1;
  printf("[PID %d] A sosit un nou client de la %s...\n", getpid(), ConvAddr());
  
  while(1)
  {
    FD_ZERO(&scriere);
    FD_SET(cd, &scriere);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    /*select cu multimea descriptorilor de scriere*/
    if(!select(cd + 1, NULL, &scriere, NULL, &tv))
      break;
    /*se poate scrie, scriem efectiv */
    if (s == 1)
      if(write(cd, MESSAGE, MES_LEN) == 0)
	break;
    FD_ZERO(&citire);
    FD_SET(cd, &citire);
    
    /*asteptam maxim 5 secunde...*/
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    /*select cu multimea descriptorilor de citire*/
    if(!select(cd + 1, &citire, NULL, NULL, &tv))
    {
      s = 0;
      continue;
    }
    s = 1;
    bzero(buffer, 100);
    /*putem citi */
    if(read(cd, buffer, 100) == 0)
      break;
    printf("[PID %d] Clientul de la %s a trimis %s", getpid(), ConvAddr(), buffer);
    if(buffer[strlen(buffer) -1] != '\n')
      printf("\n");
    
    
    puts(buffer);
    printf("Bufferul are lungimea %d\n", strlen(buffer));
    /*verificam "comanda"*/
    if(!strcmp(buffer, "varsta\n"))
    {
      strcpy(buffer, "\0");
      func1(buffer);
    }
    /*incercam sa scriem, merge!*/
  if(write(cd, buffer, 100) == 0)
    break;
  puts("s-a trimis mesaju'\n");
  }
  
  
  
  /*inchidem conexiunea cu clientul*/
  shutdown(cd, 2);
  close(cd);
  printf("[PID %d] Clientul de la %s s-a deconectat\n", getpid(), ConvAddr());
  exit(0);
}

int main(int argc, char *argv[])
{
  
  /*initializare baza de date*/
strcpy(pers[0].nume, "Popescu");
strcpy(pers[0].prenume, "Ion");
strcpy(pers[0].localitate, "Bucuresti");
pers[0].varsta = 45;
strcpy(pers[1].nume, "Ionescu");
strcpy(pers[1].prenume, "Adrian");
strcpy(pers[1].localitate, "Ploiesti");
pers[1].varsta = 25;
strcpy(pers[2].nume, "Eminescu");
strcpy(pers[2].prenume, "Mihai");
strcpy(pers[2].localitate, "Bucuresti");
pers[2].varsta = 35;
strcpy(pers[3].nume, "Constantin");
strcpy(pers[3].prenume, "Liviu");
strcpy(pers[3].localitate, "Bucuresti");
pers[3].varsta = 53;
strcpy(pers[4].nume, "Cornel");
strcpy(pers[4].prenume, "Burticel");
strcpy(pers[4].localitate, "Iasi");
pers[4].varsta = 18;

/***************************/
  
  
  
  struct sockaddr_in server;
  int i, max;
  
  if(argc == 2)
    port = atoi(argv[1]);
  /*tratare semnale*/
  if(signal(SIGCHLD, SIG_IGN) == SIG_ERR)
  {
    perror("signal()");
    return errno;
  }
  
  /*creare socket*/
  if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("socket()");
    return errno;
  }
  
  /*completare structura*/
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  /*atasare la port*/
  if(bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1 )
  {
    perror("bind()");
    return errno;
  }
  
  /*setare pentru ascultare*/
  if(listen(sd, 5) == -1)
  {
    perror("listen()");
    return errno;
  }
  
  printf("Asteptam clienti la portul %d ...\n", port);
  while(1)
  {
    FD_ZERO(&citire);
    FD_SET(sd, &citire);
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    if(select(sd + 1, &citire, NULL, NULL, &tv) == 0 )
      continue;
    l = sizeof(address);
    bzero(&address, sizeof(address));
    
    /*acceptarea unui nou client*/
    cd = accept(sd, (struct sockaddr *) &address, &l);
    
    /*lansarea unui alt proces care sa trateze cererile clientilor*/
    switch(fork())
    {
      case -1:
	shutdown(cd, 2);
	close(cd);
	perror("fork() -2");
	continue;
      case 0:
	tratare_client();
      default:
	break;
    }
  }
}
    