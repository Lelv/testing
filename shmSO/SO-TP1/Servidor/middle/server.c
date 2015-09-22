#include "../../commons/com/clsv.h"
#include "../../commons/API.h"
#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
#include <sys/stat.h>
#include<string.h>


static char* nurseS;
static char* nurseR;

static pthread_t ntid[3];
static sem_t nursePillow, healingMachine, pipeS , pipeR;
static pthread_mutex_t connectionMutex;

#define DONE 1




int processPacket(PACKET*);
void createNurses();
void doServer();
void* handleConnection(void*);
void doNurse();
void* nurse();
void fatal(char*);
void done(int);

int main (void )
{
	createNurses();
	createServer();
	pthread_mutex_init(&connectionMutex,NULL);
	while(1)
	{
		doServer();
	}

}

void createNurses() 
{		
	printf("creating nurses\n");
	nurseS = "/tmp/nurseS";
	if ( access(nurseS, 0) == -1 && mknod(nurseS, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod  nurseS");
	nurseR = "/tmp/nurseR";
	if ( access(nurseR, 0) == -1 && mknod(nurseR, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod nurseR");

		int i;
		sem_init(&nursePillow,0,0);
		sem_init(&healingMachine,0,3);
		sem_init(&pipeS,0,1);
		sem_init(&pipeR,0,1);
			
		for( i = 0 ; i < 3 ; i++ ){
			pthread_create(	&(ntid[i]) , NULL , nurse , NULL );
			printf("new nurse\n");
		}
		
	return ;
}

void* nurse()
{
	while(1)
		doNurse();
}


void doNurse()
{

	TRAY tray;

	printf("nurse sleeping\n");
	sem_wait(&nursePillow);
	printf("wake up!\n");
	int rfd, wfd;
	
	//read from nurseR
	printf("reading from nurseR thread: %d\n",pthread_self());
	rfd = open(nurseR,O_RDONLY);
	int n;
	n = read(rfd,&tray,sizeof(TRAY));
	if(n<0) {
		perror("ERROR READING");
		exit(1);
	}
	close(rfd);
	printf("done reading thread: %d\n",pthread_self());
	sem_post(&pipeS);
	//heal
	curar_pokemones(tray.pokemons,tray.cant);
	
	//write in nurseS
	printf("writing on nurseS thread: %d\n",pthread_self());
	sem_wait(&pipeR);
	pthread_kill(tray.id,DONE);
	wfd = open(nurseS,O_WRONLY);
	n = write(wfd,&tray,sizeof(TRAY));
	if(n<0) {
			perror("ERROR WRITING");
			exit(1);
	}
	printf("done writing thread: %d\n",pthread_self());
	close(wfd);
	
}	


void doServer() 
{
	//printf("hi i'm a server\n");
	CONNECTION c; 
	pthread_mutex_lock(&connectionMutex);
	acceptConnection(&c);
	//printf("connection accepted\n");
	pthread_t stid;
	pthread_create(&(stid),NULL,&handleConnection,(void*)&c);
	//printf("there's a new thread\n");
	pthread_detach(stid);
}

void* handleConnection(void* s)
{
	signal(DONE,done);
	//printf("hello\n");
	CONNECTION c;
	memcpy(&c,s,sizeof(CONNECTION));
	pthread_mutex_unlock(&connectionMutex);
	PACKET p;
	//transport
	receivePacket( &c , &p , sizeof(PACKET) );
	processPacket( &p );
	//transport
	sendPacket( &c , &p , sizeof(PACKET) );
	closeConnection( &c );
	int ret = 1;
	//printf("die thread die\n");
	pthread_exit(&ret);
}

void done(int sig)
{
	printf("DONE\n");
	signal(DONE,done);
}


int processPacket( PACKET* p )
{
	int opc = p->opc;
	//printf("opc: %d\n",opc);
	TRAY tray;
	CLSV_POKEMON_TRANSFER* pr;
	SVCL_POKEMON_TRANSFER* ps;
	
	pr =(CLSV_POKEMON_TRANSFER*) &( p->data.clsv_pokemon_transfer);
	ps =(SVCL_POKEMON_TRANSFER*) &( p->data.svcl_pokemon_transfer);		
	
	switch( opc ) 
	{
		case CURAR:
		{	

			//printf("packet : \n");
			//printf("pr->cant:%d\n",pr->cant);
			tray.cant = pr->cant;
			memcpy(tray.pokemons,pr->pokemons,tray.cant*sizeof(POKEMON));

			tray.id = pthread_self();
			sem_wait(&healingMachine);
			int wfd, rfd;			
			
			
			//write nurseR;

			sem_wait(&pipeS);
			printf("writing on nurseR thread: %d\n",pthread_self());
			sem_post(&nursePillow);
			wfd = open(nurseR,O_WRONLY);
			int n = write(wfd,&tray,sizeof(TRAY));
			//printf("n: %d\n",n);
			if(n<0) {
				perror("ERROR WRITING");
				exit(1);
			}
			close(wfd);
			//printf("done writing thread: %d\n",pthread_self());
			//printf("ZZZZZZZZ thread: %d\n",pthread_self());
			

			pause();
			//printf("up thread: %d\n",pthread_self());
			//read nurseS;

			rfd = open(nurseS,O_RDONLY);
			printf("reding from nurseS thread: %d\n",pthread_self());
			n = read(rfd,&tray,sizeof(TRAY));
			if(n<0) {
				perror("ERROR READING");
				exit(1);
			}
			close(rfd);
			
			//printf("done reading thread: %d\n",pthread_self());
			sem_post(&pipeR);
			sem_post(&healingMachine);
				

			memcpy(pr->pokemons,tray.pokemons,tray.cant*sizeof(POKEMON));
			
			break;
		}
		case ADOPTAR:
		{
			//call adoptar;
			break;			
		}
		case REGALAR:
		{
			//call regalar;
			break;
		}
		default:
		{
			//error;
		}
	}
}

void
fatal(char *s)
{
	perror(s);
	exit(1);
}
