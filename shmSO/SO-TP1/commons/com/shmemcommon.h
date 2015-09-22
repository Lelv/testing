#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

typedef enum { false, true } bool;

void terminal(char *s);
char * getmem(void);
void initmutex(void);
void enter(void);
void leave(void);

