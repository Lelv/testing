#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../commons/myTypes.h"
#define forever while(1)

void generatePokemons(POKEMON pokemones[6]);
void printPokemons(POKEMON pokemones[6]);
void printLogo();
void printHelp();

int main(){

	POKEMON pokemones[6];
	generatePokemons(pokemones);

	printf("\033[2J\033[1;1H");
	printLogo();
	printf("\t\t    ---- POKEMON CENTER ----\n\n");
	printf("Your pokemons are: \n");
	printPokemons(pokemones);


	char buffer[256];

	forever{
		printf("\n--> What do you want to do?\n$ ");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);

		if(strcmp(buffer,"heal\n") == 0){
			curar_pokemones(pokemones, 6);
		}else if (strcmp(buffer, "new pokemons\n") == 0)
		{
			generatePokemons(pokemones);
			printf("Your new pokemons are\n");
			printPokemons(pokemones);
		}else if(strcmp(buffer, "my pokemons\n") == 0){
			printPokemons(pokemones);
		}else if((strcmp(buffer, "help\n")) == 0 || (strcmp(buffer, "man\n")) == 0){
			printHelp();
		}else{
			printf("Invalid command\n");
		}

		
	
  }

}

void generatePokemons(POKEMON pokemones[6]){
	srand(time(NULL));
	int i;
	int cant = rand()%3 + 4;
	for(i = 0; i < cant; i++){
		strcpy(pokemones[i].name, POKEMON_NAMES[rand()%150]);
		pokemones[i].life = rand()%70 + 1;
	}
	for(i = cant; i < 6; i++){
		pokemones[i].life = -1;
	}

}

void printPokemons(POKEMON pokemones[6]){
	int i;
	int j = 1;
	for (i= 0; i<6 && pokemones[i].life != -1; i++){
		printf("   %d) %-15s---   Life: %d%% \n", j, pokemones[i].name, pokemones[i].life);
		j++;
	}

}

void printLogo(){
	printf("                                .::.                          \n");
	printf("                              .;:**'                          \n");
	printf("                              `                               \n");
	printf("  .:XHHHHk.              db.   .;;.     dH  MX                \n");
	printf("oMMMMMMMMMMM       ~MM  dMMP :MMMMMR   MMM  MR      ~MRMN     \n");
	printf("QMMMMMb  \"MMX       MMMMMMP !MX' :M~   MMM MMM  .oo. XMMM 'MMM\n");
	printf("  `MMMM.  )M> :X!Hk. MMMM   XMM.o\"  .  MMMMMMM X?XMMM MMM>!MMP\n");
	printf("   'MMMb.dM! XM M'?M MMMMMX.`MMMMMMMM~ MM MMM XM `\" MX MMXXMM \n");
	printf("    ~MMMMM~ XMM. .XM XM`\"MMMb.~*?**~ .MMX M t MMbooMM XMMMMMP \n");
	printf("     ?MMM>  YMMMMMM! MM   `?MMRb.    `\"\"\"   !L\"MMMMM XM IMMM  \n");
	printf("      MMMX   \"MMMM\"  MM       ~%%:           !Mh.\"\"\" dMI IMMP  \n");
	printf("      'MMM.                                             IMX   \n");
	printf("       ~M!M                                             IMP   \n");
	//printf("aaaaaaaaaaaa\n");
}

void printHelp(){
	printf("***\tHelp\t***\n");
	printf("Available commands:\n");
	printf("--> heal: Heal all your pokemons to full health\n");
	printf("--> my pokemons: Check your pokemon's health\n");
	printf("--> new pokemons: Receive new pokemons\n");
}