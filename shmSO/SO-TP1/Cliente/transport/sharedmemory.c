#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../commons/com/clserv.h"
#include "../../commons/com/shmemcommon.h"

char * memoria;

int connect_to(char* hostname, char* port);

int connect_to_server() {
	memoria = getmem();
	initmutex();
}


int send_packet(void* p, int size )
{	
	enter();
	memcpy(memoria, p, size);
	leave();
}

int receive_packet(void* p, int size )
{
	enter();
	memcpy(p, memoria, size);
	leave();
}


int connect_to(char* hostname, char* port) {
	
}
































