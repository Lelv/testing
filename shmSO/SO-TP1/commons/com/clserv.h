

#include "../myTypes.h"
typedef int OPC;

enum
{
	CURAR = 0 , REGALAR = 1, ADOPTAR = 2, RET_CURAR=3
};



			/* Client -> server	*/

typedef struct
{
	POKEMON pokemons[6];
	int cant;
	CLIENT_ID id;
}	CLSV_POKEMON_TRANSFER;


			/* Server -> client	*/

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

typedef struct
{
	OPC opc;
	DATA data;
} CLSVBUFF;
	
int
send_packet( void *p, int qty );


int
receive_packet( void *p, int lim );


int
connect_to_server(void);

int
creatServ(void);

int killServer();




