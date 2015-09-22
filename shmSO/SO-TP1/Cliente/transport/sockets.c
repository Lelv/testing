#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include "../../commons/com/clserv.h"

static int serv;

int connect_to(char* hostname, char* port);
int connect_to_server() {
	serv = connect_to("127.0.0.1","5001");
	return serv;
}


int send_packet(void* p, int size )
{	
	//COMENTDEBUG
	//printf("sending packet\n");
	int sockfd = serv;
	int n = write(sockfd,p,size);
	if (n < 0)
   {
      perror("ERROR writing to socket");
      return -1;
   }
	return n;
}

int receive_packet(void* p, int size )
{
	//COMENTDEBUG
	//printf("receiving packet\n");
	int sockfd = serv;
	int n = read(sockfd, p,size);
	if (n < 0)
   {
      perror("ERROR reading from socket");
      return -1;
   }

	return n;

}


int connect_to(char* hostname, char* port) {
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
   

   	portno = atoi(port);
   
   /* Create a socket point */
  	 sockfd = socket(AF_INET, SOCK_STREAM, 0);
  	 
  	 if (sockfd < 0)
  	 {
  	    perror("ERROR opening socket");
  	    return -1;
  	 }
	 server = gethostbyname(hostname);
   
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		return -2;
	}
	   
	   bzero((char *) &serv_addr, sizeof(serv_addr));
	   serv_addr.sin_family = AF_INET;
	   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	   serv_addr.sin_port = htons(portno);
	   
	   /* Now connect to the server */
	   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	   {
		  perror("ERROR connecting");
		  exit(1);
	   }
	return sockfd;
}
































