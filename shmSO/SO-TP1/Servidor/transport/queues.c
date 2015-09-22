





int creatServ() {

mqd_t qin, qout;
int n;
char *servname = "/server_queue";
char cltname[100];
struct
{
	long mtype;
	char mtext[200];
} 
msg;
char *msgptr = (char *) &msg;
int offset = msg.mtext - msgptr;
struct mq_attr attr;
attr.mq_maxmsg = 10;
attr.mq_msgsize = sizeof msg;
	
if ( (qin = mq_open(servname, O_RDONLY|O_CREAT, 0666, &attr)) == -1 )
	fatal("Error mq_open qin");


}
