#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../commons/com/clsv.h"
#include "../../commons/com/shmemcommon.h"
#include <signal.h>

char * memoria;
char * last;

void createServer()
{
	printf("JOJOJO\n");
	memoria = getmem();
	memset(memoria, 0, sizeof(PACKET));
	initmutex();
	printf("JAJAJAJA\n");
}

void acceptConnection(CONNECTION* c)
{
	


}

int receivePacket(CONNECTION* c, PACKET *p, int lim ) {
	enter();
	if ( memcmp(last, memoria, lim) )
	{
		memcpy(p, memoria, lim);
		memcpy(last, memoria, lim);
	}
	leave();
	return 1;
}

int sendPacket(CONNECTION* c, PACKET *p , int qty ) {
	printf("sending packet\n");
	enter();
	memcpy(memoria, p, qty);
	leave();
	return 1;
}

int closeConnection(CONNECTION* c)
{
	
}

void killServer(int signo)
{
	shm_unlink(memoria);
}












