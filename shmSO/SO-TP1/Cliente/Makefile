#Makefile

.SILENT:

TARGET = trainer
OBJS = ./app/trainer.o ./middle/clstub.o
SOCKETS = ./transport/sockets.o
PIPES = ./transport/pipes.o 
SHMEM = ./transport/sharedmemory.o ../commons/com/shmemcommon.o
CC = gcc
LD = $(CC)
LDOPTS = -o
LIBS = -lpthread -lrt





$(TARGET):
sockets: $(OBJS) $(SOCKETS)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS) $(SOCKETS)
pipes: $(OBJS) $(PIPES)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS) $(PIPES)
shmem: $(OBJS) $(SHMEM)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS) $(SHMEM) $(LIBS)


trainer.o:	../commons/myTypes.h

clean:
	rm -rf *.o $(TARGET)
	cd app; rm -rf *.o
	cd middle; rm -rf *.o
	cd transport; rm -rf *.o
	cd ../commons/com/; rm -rf *.o
