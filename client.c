
//Client program:
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>


//function declaration and global variable declaration
void *recvmg(void *);
char msg[500];


int main(int argc,char *argv[])
{
	pthread_t recvt;
	int len;
	int sock;
	char send_msg[500];

	struct sockaddr_in ServerIp;
	char client_name[100];
	strcpy(client_name, argv[1]);

	//creating socket at server
	sock = socket( AF_INET, SOCK_STREAM,0);
	if(sock<0)
        printf("Error in opening socket \n");
	ServerIp.sin_port = htons(1234); //The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
	ServerIp.sin_family= AF_INET;
	ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");//local host IP address

	//trying to connect with server
	if( (connect( sock ,(struct sockaddr *)&ServerIp,sizeof(ServerIp))) == -1 )
		perror("ERROR: connection failed");

	//creating a client thread which is always waiting for a message
	pthread_create(&recvt,NULL,(void *)recvmg,&sock);

	//ready to read a message from console
	while(fgets(msg,500,stdin) > 0)
        {
            strcpy(send_msg,client_name);
            strcat(send_msg,":");
            strcat(send_msg,msg);
            len = write(sock,send_msg,strlen(send_msg));
            if(len < 0)
                perror("/n message failed to send /n");
        }

	//thread is closed
	pthread_join(recvt,NULL);
	close(sock);//close socket
	return 0;
}

void *recvmg(void *my_sock)
{
	int sock = *((int *)my_sock);
	int len;
	// client thread always ready to receive message
	while((len = recv(sock,msg,500,0)) > 0)
        {
            msg[len] =' ';
            fputs(msg,stdout);
	   }
}
