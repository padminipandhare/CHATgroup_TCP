//Server program :

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>

pthread_mutex_t mutex;
int clients[5];
int n=0;

void sendtoall(char *msg,int curr)
     {
        int i;
        pthread_mutex_lock(&mutex);
        for(i = 0; i < n; i++)
            {
            if(clients[i] != curr)
                {
                    if(send(clients[i],msg,strlen(msg),0) < 0)
                        {
                        printf("message send to fail \n");
                        continue;
                       }
                }
           }
        pthread_mutex_unlock(&mutex);
    }

void *recvmg(void *client_sock)
   {
	int sock = *((int *)client_sock);
	char msg[500];
	int len;

	while((len = recv(sock,msg,500,0)) > 0)
           {
                msg[len] =' ';
                sendtoall(msg,sock);
           }

   }

int main()
{

  struct sockaddr_in ServerIp;
  pthread_t recvt;
  int sock=0 , Client_sock=0;

   ServerIp.sin_family = AF_INET;
   ServerIp.sin_port = htons(1234); //port number
   ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");//local host IP address number for checking purpose only

   //creating socket
   sock = socket( AF_INET , SOCK_STREAM, 0 );
   if(sock<0)
        perror("Error in opening socket");
   //bind address to a socket
   if( bind( sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp)) == -1 )
		perror("ERROR:cannot bind! \n");
   else
		printf("Server bind\n");
    //listen from client and also decide limit of connection
   if( listen( sock ,5 ) == -1 )
		perror("ERROR :listen Failed \n");
   else
        printf("WELCOME for chat !\n");

   while(1)
    {
        /*The accept function is called by a TCP server to return the next completed connection
         from the front of the completed connection queue */
		if( (Client_sock = accept(sock, (struct sockaddr *)NULL,NULL)) < 0 )
			printf("accept connection failed\n");

		pthread_mutex_lock(&mutex);
		clients[n]= Client_sock;
		n++;
		// creating a thread for each client
		pthread_create(&recvt,NULL,(void *)recvmg,&Client_sock);
		pthread_mutex_unlock(&mutex);
	}
	return 0;

}
