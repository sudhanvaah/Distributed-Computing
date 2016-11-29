#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define BACKLOG 10
int main()
{
int listenfd,connfd;
struct sockaddr_in servaddr;
char buff[1000];
time_t ticks_req,ticks_res;
//STEP 1: socket function
listenfd = socket(AF_INET,SOCK_STREAM,0);
printf("SERVER: Listening socket opened with listenfd = %d\n",listenfd);
//STEP 2: initialize address structure
bzero(&servaddr, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
servaddr.sin_port = htons(5000);
//STEP 3: bind (int) listenfd and (struct sockaddr_in) servaddr
bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
//STEP 4: start listening to client requests
listen(listenfd,BACKLOG);
for( ; ; )
{
//accept client request
connfd = accept(listenfd,(struct sockaddr *)NULL,NULL);
if(fork()==0)
{
ticks_req = time(NULL);

close(listenfd);
printf("SERVER: Connection established with connfd=%d\n", connfd);
ticks_res = time(NULL);

snprintf(buff,sizeof(buff),"Request: %s\nResponse: %s\n",ctime(&ticks_req),ctime(&ticks_res));
write(connfd,buff,strlen(buff));//send data to client
close(connfd);//close client connection
exit(0);//end child process
}
close(connfd);//close client connection with original server process
}
close(listenfd);//close listening socket
}