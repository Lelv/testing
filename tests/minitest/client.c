#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "common.h"

#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <mqueue.h>

void generatePokemons(POKEMON pokemones[6]);
void printPokemons(POKEMON pokemones[6]);
void printLogo();
void printHelp();

typedef enum { false, true } bool;

void
fatal(char *s)
{
	perror(s);
	exit(1);
}

int
main(int argc, char **argv)
{
	mqd_t qin, qout;
	int n;
	char *servname = "/server_queue";
	char cltname[100];

	TRAINER msg;
	generatePokemons(msg.pokemones);
	printf("Your pokemons are: \n");
	printPokemons(msg.pokemones);

	char buffer[MAX_LENGTH];

	char *msgptr = (char *) &msg;
	int offset = msg.mtext - msgptr;
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof msg;
		
	sprintf(cltname, "/client_%d_queue", getpid());
	if ( (qin = mq_open(cltname, O_RDONLY|O_CREAT, 0666, &attr)) == -1 )
		fatal("Error mq_open qin");
	if ( (qout = mq_open(servname, O_WRONLY|O_CREAT, 0666, &attr)) == -1 )
		fatal("Error mq_open qout");
		
	while ( (n = read(0, msg.mtext, MAX_LENGTH)) > 0 )
	{
		msg.mtype = getpid();
		printf("Cliente envia: %.*s", n, msg.mtext);
		mq_send(qout, msgptr, n + offset, 0);
		n = mq_receive(qin, msgptr, sizeof msg, NULL);
		if ( n > 0 )
			printf("Cliente recibe: %.*s", n - offset, msg.mtext);
	}
	printf("Cliente termina\n");
	mq_unlink(cltname);
	return 0;
 }
 /*
int main(){

	POKEMON pokemones[6];
	generatePokemons(pokemones);

	printf("\033[2J\033[1;1H");
	//printLogo();
	printf("\t\t    ---- POKEMON CENTER ----\n\n");
	printf("Your pokemons are: \n");
	printPokemons(pokemones);


	char buffer[256];

	forever{
		printf("\n--> What do you want to do?\n$ ");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);

		if(strcmp(buffer,"heal\n") == 0){
			//curar_pokemones(pokemones, 6);
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

}*/

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