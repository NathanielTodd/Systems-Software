#include  <string.h>
#include  <sys/types.h>
#include  <netinet/in.h>
#include  <arpa/inet.h>
#include  <stdio.h>
#include  <sys/socket.h>

int main(){

	int sfd;
	if((sfd = socket(PF_INET,SOCK_STREAM,0))==-1){
		printf("Error on Socket Initialization\n");
		return -1;
	}
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(50380);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
	listen(sfd, 10);

	int connfd = accept(sfd, NULL, NULL);
	
	char buffer[1024];
	strcpy(buffer, "Hello there!\n");
	send(connfd, buffer, strlen(buffer), 0);
	
	char inbuff[1024];
	int ret = recv(connfd,&inbuff,sizeof(inbuff),0);
	printf("buffer = %s",&inbuff);
	strcpy(buffer,"Hi, What's your name?\n");
	send(connfd, buffer, strlen(buffer),0);
	
	close(connfd);
	close(sfd);
	
}
