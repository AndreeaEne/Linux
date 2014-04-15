#include <unistf.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAX_CLIENTS 10 /*numar maxim de clienti*/
#define FULL "Nu mai pot fi acceptati alti clienti...\n"
#define MESSAGE "Mesajul"
#define MES_LEN strlen(MESSAGE)