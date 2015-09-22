#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define MAX_LENGTH 256

#define NQUEUE 5
#define NMACHINES 3

typedef struct
{	
	char name[MAX_LENGTH];
	int life;
} POKEMON;

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static int waiting[NQUEUE];
static POKEMON waitingPkmn[NQUEUE][6];
static pthread_cond_t clients_waiting, client_wakeup[NQUEUE];
static int client_id = 1;
static int machine_id = 1;


static const char* POKEMON_NAMES[] = {"Bulbasaur", "Ivysaur", "Venusaur", "Charmander", 
"Charmeleon", "Charizard", "Squirtle", "Wartortle", "Blastoise", "Caterpie", "Metapod", 
"Butterfree", "Weedle", "Kakuna", "Beedrill", "Pidgey", "Pidgeotto", "Pidgeot", "Rattata", 
"Raticate", "Spearow", "Fearow", "Ekans", "Arbok", "Pikachu", "Raichu", "Sandshrew", "Sandslash", 
"Nidoran♀", "Nidorina", "Nidoqueen", "Nidoran♂", "Nidorino", "Nidoking", "Clefairy", "Clefable", 
"Vulpix", "Ninetales", "Jigglypuff", "Wigglytuff", "Zubat", "Golbat", "Oddish", "Gloom", 
"Vileplume", "Paras", "Parasect", "Venonat", "Venomoth", "Diglett", "Dugtrio", "Meowth", "Persian", 
"Psyduck", "Golduck", "Mankey", "Primeape", "Growlithe", "Arcanine", "Poliwag", "Poliwhirl", 
"Poliwrath", "Abra", "Kadabra", "Alakazam", "Machop", "Machoke", "Machamp", "Bellsprout", 
"Weepinbell", "Victreebel", "Tentacool", "Tentacruel", "Geodude", "Graveler", "Golem", "Ponyta", 
"Rapidash", "Slowpoke", "Slowbro", "Magnemite", "Magneton", "Farfetch'd", "Doduo", "Dodrio", 
"Seel", "Dewgong", "Grimer", "Muk", "Shellder", "Cloyster", "Gastly", "Haunter", "Gengar", "Onix", 
"Drowzee", "Hypno", "Krabby", "Kingler", "Voltorb", "Electrode", "Exeggcute", "Exeggutor", "Cubone", 
"Marowak", "Hitmonlee", "Hitmonchan", "Lickitung", "Koffing", "Weezing", "Rhyhorn", "Rhydon", 
"Chansey", "Tangela", "Kangaskhan", "Horsea", "Seadra", "Goldeen", "Seaking", "Staryu", "Starmie", 
"Mr. Mime", "Scyther", "Jynx", "Electabuzz", "Magmar", "Pinsir", "Tauros", "Magikarp", "Gyarados", 
"Lapras", "Ditto", "Eevee", "Vaporeon", "Jolteon", "Flareon", "Porygon", "Omanyte", "Omastar", 
"Kabuto", "Kabutops", "Aerodactyl", "Snorlax", "Articuno", "Zapdos", "Moltres", "Dratini", 
"Dragonair", "Dragonite", "Mewtwo", "Mew"};



void generatePokemons(POKEMON pokemones[6]){
	//pthread_mutex_lock(&mut);
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
	//pthread_mutex_unlock(&mut);

}

void printPokemons(POKEMON pokemones[6]){
	int i;
	int j = 1;
	for (i= 0; i<6 && pokemones[i].life != -1; i++){
		printf("   %d) %-15s---   Life: %d%% \n", j, pokemones[i].name, pokemones[i].life);
		j++;
	}

}

static int
get_client(void)
{
	int id = 0, chair = -1;
	int i, n;

	/* Para respetar el orden de llegada, recorro las sillas y elijo
	   el cliente con el id más bajo */	
	for ( i = 0 ; i < NQUEUE ; i++ )
		if ( (n = waiting[i]) && (!id || n < id) )
		{
			id = n;
			chair = i;
		}
	/* Despertamos al cliente */
	if ( chair != -1 )
	{
		pthread_cond_signal(&client_wakeup[chair]);
		waiting[chair] = 0;
	}

	return id;
}

static void curar_pokemones(POKEMON pokemones[6], int cant){
	int i;
	for(i = 0; i<cant && pokemones[i].life != -1; i++){
		printf("Curando a: %s\n", pokemones[i].name );
		usleep(15000* (100-pokemones[i].life));
		pokemones[i].life = 100;
	}
}

static void
cut_hair(int barb, int clt)
{
	printf("Peluquero %d: atiendo cliente %d\n", barb, clt);
	usleep(1000000 + rand() % 2000000);
	printf("Peluquero %d: cliente %d atendido\n", barb, clt);
}

static void *
machine(void *arg)
{
	int id;

	pthread_mutex_lock(&mut);
	id = machine_id++;
	pthread_mutex_unlock(&mut);
	printf("Peluquero %d: creado\n", id);

	while ( 1 )
	{
		int n;

		pthread_mutex_lock(&mut);
		/* Si hay algún cliente esperando, despertarlo */
		while ( !(n = get_client()) )
		{
			/* Si no, a dormir */
			printf("Peluquero %d: esperando cliente\n", id);
			pthread_cond_wait(&clients_waiting, &mut);
		}
		pthread_mutex_unlock(&mut);

		/* Cortar el pelo */
		cut_hair(id, n);
	}

	return NULL;
}

static int 
get_chair(void)
{
	int i;

	/* Buscar una silla libre */
	for ( i = 0 ; i < NQUEUE ; i++ )
		if ( !waiting[i] )
			return i;
	return -1;
}

static void *
client(void *arg)
{
	int id, n;

	pthread_mutex_lock(&mut);

	POKEMON pkmn[6];
	generatePokemons(pkmn);
	printPokemons(pkmn);

	id = client_id++;
	printf("Cliente %d: creado\n", id);

	/* Si hay una silla libre, sentarse */
	if ( (n = get_chair()) != -1 )
	{
		waiting[n] = id;
		/* Eventualmente despertar un peluquero */
		pthread_cond_signal(&clients_waiting);
		printf("Cliente %d: espero en la silla %d\n", id, n);
		/* Dormir */
		pthread_cond_wait(&client_wakeup[n], &mut);
		printf("Cliente %d: me atiende un peluquero\n", id);
	}
	else
	/* Si no, irse */
		printf("Cliente %d: no hay sillas libres\n", id);

	pthread_mutex_unlock(&mut);

	return NULL;
}

static void
create_machines(void)
{
	pthread_t machine_thr;

	pthread_create(&machine_thr, NULL, machine, NULL);
	pthread_detach(machine_thr);
}

static void
create_client(void)
{
	pthread_t trainer_thr;
	
	//POKEMON pkmn[6];
	//generatePokemons(pkmn);
	//pthread_create(&trainer_thr, NULL, client, pkmn);

	pthread_create(&trainer_thr, NULL, client, NULL);
	pthread_detach(trainer_thr);
}


int main(){
	int c,i;

	// Crear los peluqueros
	for ( i = 0 ; i < NMACHINES ; i++ )
		create_machines();

	while ( (c = getchar()) != EOF ){
		create_client();
	}
}