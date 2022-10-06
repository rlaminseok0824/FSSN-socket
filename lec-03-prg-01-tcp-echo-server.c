#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define BUFFSIZE 200
#define CLNT_MAX 10

int g_client_socket[CLNT_MAX];
int g_client_count = 0;
int g_port = 65456;

void send_all(char * msg,int my_sock){
	write(my_sock,msg,strlen(msg) + 1);

//	for(int i = 0; i < g_client_count;i++){
//		write(g_client_socket[i],msg,strlen(msg)+1);
//	}
}

int main(int argc,char ** argv){
	printf("> echo-server is activated\n");

	int server_socket;
	struct sockaddr_in server_addr;

	int client_socket;
	struct sockaddr_in client_addr;
	int client_addr_size;

	server_socket = socket(PF_INET,SOCK_STREAM,0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(g_port);

	bind(server_socket,(struct sockaddr *)&server_addr,sizeof(server_addr));

	listen(server_socket,5);
	


	int recv_len = 0;
	char buff[BUFFSIZE];
//	int flag = 0;
	while(1){
		client_addr_size = sizeof(client_addr);
		client_socket = accept(server_socket,(struct sockaddr *)&client_addr,&client_addr_size);
		//printf("client connected!!\n");	
		g_client_socket[g_client_count++] = client_socket;
		
		printf("> client connected by IP address %s with Port number %d\n",inet_ntoa(client_addr.sin_addr),client_addr.sin_port);
		
		while(1){
			recv_len = read(client_socket,buff,BUFFSIZE);
		
			if(recv_len == -1){
				continue;
			}
			printf("> echoed: %s\n",buff);
			send_all(buff,client_socket);
			if(strcmp(buff,"quit") == 0){
				//flag = 1;
				break;
			}
		}
//		if(flag){
//			break;
//		}
	}
	printf("> echo-server is de-activated\n");
	return 1;
}
