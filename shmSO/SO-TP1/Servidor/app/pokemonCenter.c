#include "../../commons/myTypes.h"
#include <stdio.h>
#include <unistd.h>

int curar_pokemones(POKEMON *pokemones, int cant){
	int i;
	for(i = 0; i<cant && pokemones[i].life != -1; i++){
		printf("Curando a: %s\n", pokemones[i].name );
		usleep(15000* (100-pokemones[i].life));
		pokemones[i].life = 100;
	}
	return 1;
}

