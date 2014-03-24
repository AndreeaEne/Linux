#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define PORT 8081
extern int errno;
int main()
{
struct sockaddr_in server;
struct sockaddr_in from;
char buffer[100];
int sd;
if((sd=socket(AF_INET, SOCK_STREAM, 0))==-1)
{
perror("Eroare la socket().\n");
return errno;
}
bzero(&server, sizeof(server));
bzero(&from, sizeof(from));
server.sin_family=AF_INET;
server.sin_addr.s_addr=htonl(INADDR_ANY);
server.sin_port=htons(PORT);
if(bind(sd,(struct sockaddr *) &server,sizeof(struct sockaddr))==-1)
{
perror("Eroare la bind().\n");
return errno;
}
if(listen(sd,5)==-1)
{
perror("Eroare la listen().\n");
return errno;
}
while(1)
{
int client;
int length=sizeof(from);
printf("Asteptam la portul %d...\n",PORT);
fflush(stdout);
client=accept(sd,(struct sockaddr *) &from, &length);
if(client<0)
{
perror("Eroare la accept().\n");
continue;
}
bzero(buffer,100);
printf("Asteptam mesajul...\n");
fflush(stdout);
if(read(client,buffer,100)<=0)
{
perror("Eroare la read() de la client.\n");
close(client);
continue;
}
printf("Mesajul a fost receptionat...\n" "Trimitem mesajul inapoi...");
char aux;
int i,j;
for(i=0;i<strlen(buffer)-1;i++)
for(j=i+1;j<strlen(buffer);j++)
if(buffer[i]>buffer[j])
{
aux=buffer[i];
buffer[i]=buffer[j];
buffer[j]=aux;
}
if(write(client,buffer,100)<=0)
{
perror("Eroare la write() catre client.\n");
continue;
}
else 
printf("transmitere cu succes.\n");
close(client);
}
}
