
#include<pthread.h>
#include "../myTypes.h"

enum
{
	CURAR = 0 , REGALAR = 1, ADOPTAR = 2, RET_CURAR=3
};

typedef struct
{
	POKEMON pokemons[6];
	int cant;
	CLIENT_ID id;
}	CLSV_POKEMON_TRANSFER;


typedef struct
{
	POKEMON pokemons[6];
	int cant;
	int ok;
}	SVCL_POKEMON_TRANSFER;

typedef union
{
	CLSV_POKEMON_TRANSFER clsv_pokemon_transfer;
	SVCL_POKEMON_TRANSFER svcl_pokemon_transfer;
} DATA;





typedef union
{
	int sockfd;	

}CONNECTION;

typedef struct
{
	int opc;
	DATA data;

}PACKET;



typedef struct
{
	POKEMON pokemons[6];
	int cant;
	pthread_t id;
}TRAY;


int sendPacket(CONNECTION* c, PACKET* , int );
int receivePacket(CONNECTION* c, PACKET*, int);
int closeConnection(CONNECTION*);
void acceptConnection(CONNECTION*);
void createServer();
void killServer(int);


