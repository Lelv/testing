#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../commons/com/clserv.h"

static CLSVBUFF clsvbuff;

static
int
send_rcv( CLSVBUFF *p, int opcode, int qty )
{
	//COMENTDEBUG
	//printf("sending rcv\n");
	int serv = connect_to_server();
	int qtyrec, qtysent;
	p->opc = opcode;
	qtysent = send_packet( p, qty + sizeof(OPC));
	qtyrec = receive_packet( p, sizeof( *p ));

	int ret_opcode = p->opc;
	if(ret_opcode = RET_CURAR) {
		//COMENTDEBUG
		//printf("pokemones curados\n");
		
	}
	close(serv);
	return qtyrec - sizeof ( OPC );
}

int curar_pokemones(POKEMON* pokemones,int cant) {
	//COMENTDEBUG
	//printf("curando\n");

	CLSV_POKEMON_TRANSFER * ps;
	SVCL_POKEMON_TRANSFER * pr;
	
	ps = &clsvbuff.data.clsv_pokemon_transfer;
	pr = &clsvbuff.data.svcl_pokemon_transfer;

	memcpy(ps->pokemons,pokemones,cant*sizeof(POKEMON));
	ps->cant=cant;
	ps->id=1;

		
	send_rcv(&clsvbuff,CURAR,sizeof(CLSV_POKEMON_TRANSFER));

  	memcpy(pokemones,ps->pokemons, cant*sizeof(POKEMON));

  	return 1;


}

/* regalar , adoptar */


