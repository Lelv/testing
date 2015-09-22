//fifos.c
#include <stdio.h>
#include <stdlib.h>
#include "../../commons/com/clserv.h"

static int pid, n;
static int fd;
static char *name = "/tmp/fifo";
static char buf[200];

int connect_to_server() {
	if(fd = open(name, O_RDONLY)){
		return -1;
	}
}


int send_packet(void* p, int size ){
	printf("sending packet\n");
	fd = open(name, O_WRONLY);
	n = write(fd, p, size);
	if(n > 0){
		perror("ERROR writing to fifo");
		exit(1);
	}
	return n;
}

int receive_packet(void* p, int size ){
	printf("receiving packet\n");
	n = read(sockfd, p,size);
	if (n < 0)
   {
      perror("ERROR reading from fifo");
      exit(1);
   }

	return n;
}






