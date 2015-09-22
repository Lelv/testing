#include "shmemcommon.h"
#include "clsv.h"

void
terminal(char *s)
{
	perror(s);
	exit(1);
}

char *
getmem(void)
{
	int fd;
	char *mem;
	int size = sizeof(PACKET);
	if ( (fd = shm_open("/message", O_RDWR|O_CREAT, 0666)) == -1 )
		terminal("sh_open");
	ftruncate(fd, size);
	if ( !(mem = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		terminal("mmap");
	close(fd);
	return mem;
}

static sem_t *sd;

void
initmutex(void)
{
	if ( !(sd = sem_open("/mutex", O_RDWR|O_CREAT, 0666, 1)) )
		terminal("sem_open");
}

void
enter(void)
{
	sem_wait(sd);
}

void
leave(void)
{
	sem_post(sd);
}

