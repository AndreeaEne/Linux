#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

extern int errno;
int port;

int main(int argc, char *argv[])
	{
		int sd;
		struct sockaddr_in server;
		char buffer[100];
		
		if(argc != 3)
			{
				printf("Sintaxa: %s <adresa_server> <port>\n",argv[0]);
				return -1;
			}
		while(1)
		{	
			port = atoi(argv[2]);
			
			if((sd = socket(AF_INET,SOCK_STREAM,0)) == -1)
				{
					perror("Eroare la socket().\n");
					return errno;
				}
			
			server.sin_family = AF_INET;
			server.sin_addr.s_addr = inet_addr(argv[1]);
			server.sin_port = htons(port);
			
			if(connect(sd, (struct sockaddr*) &server, sizeof(struct sockaddr)) == -1)
				{
					perror("Eroare la connect().\n");
					return errno;
				}
				
			bzero(buffer, 100);
			printf("Introduceti mesajul: ");
			fflush(stdout);
			read(0,buffer,100);
			
			if(write(sd,buffer,100) <= 0)
				{
					perror("Eroare la write()) spre server.\n");
					return errno;
				}
			if(read(sd,buffer,100) < 0)
				{
					perror("Eroare la read() de la server.\n");
					return errno;
				}
			printf("Mesajul primit este: %s \n",buffer);
		}
		close(sd);
	}
