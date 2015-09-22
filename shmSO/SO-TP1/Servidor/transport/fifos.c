//fifos.c


static char *name = "/tmp/fifo";





int creatServ(){
	if ( access(name, 0) == -1 && mknod(name, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod");
	return 1;
}
