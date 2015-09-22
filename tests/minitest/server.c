#include "common.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <mqueue.h>

int curar_pokemones(POKEMON *pokemones, int cant){
	int i;
	for(i = 0; i<cant && pokemones[i].life != -1; i++){
		printf("Curando a: %s\n", pokemones[i].name );
		usleep(15000* (100-pokemones[i].life));
		pokemones[i].life = 100;
	}
	return 1;
}

typedef enum { false, true } bool;

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

int
main(int argc, char **argv)
{
	mqd_t qin, qout;
	int n;
	char *servname = "/server_queue";
	char cltname[100];
	struct
	{
		long mtype;
		char mtext[200];
	} 
	msg;
	char *msgptr = (char *) &msg;
	int offset = msg.mtext - msgptr;
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof msg;
		
	if ( (qin = mq_open(servname, O_RDONLY|O_CREAT, 0666, &attr)) == -1 )
		fatal("Error mq_open qin");
		
	while ( true )
		if ( (n = mq_receive(qin, msgptr, sizeof msg, NULL)) > 0 )
		{
			printf("Servidor: %.*s", n - offset, msg.mtext);
			sprintf(cltname, "/client_%ld_queue", msg.mtype);
			if ( (qout = mq_open(cltname, O_WRONLY)) == -1 )
				fatal("Error mq_open qout");
			strcpy(msg.mtext, "todo bien");
			mq_send(qout, msgptr, n, 0);
		}
 }

