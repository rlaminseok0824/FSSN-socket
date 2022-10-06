#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define BUFFSIZE 200

int port = 65456;

int main(int argc,char ** argv){
	printf("> echo-client is activated\n");
	int client_socket;
	struct sockaddr_in server_addr;

	
	client_socket = socket(PF_INET,SOCK_STREAM,0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	server_addr.sin_port=htons(port);
	
	if(connect(client_socket,(struct sockaddr *)&server_addr,sizeof(server_addr))== -1){
		printf("connection error!\n");
	}
	printf("connection success!\n");
	
	char msg[BUFFSIZE];
	char recv_msg[BUFFSIZE];
	int readlen = 0;
	while(1){
		printf("> ");
		scanf("%s",&msg);
		//printf("send : %s",msg);
		send(client_socket,msg,strlen(msg) + 1,0);
		readlen = recv(client_socket,recv_msg,BUFFSIZE,0);
		printf("> received: %s\n",recv_msg);
		if(strcmp("quit",msg) == 0){
			break;
		}
	}	
	printf("> echo-client is de-activated\n");
	return 1;
}
