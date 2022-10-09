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
//int g_port = 65456;

void send_all(char * msg,int my_sock){
	write(my_sock,msg,strlen(msg) + 1);

//	for(int i = 0; i < g_client_count;i++){
//		write(g_client_socket[i],msg,strlen(msg)+1);
//	}
}

typedef struct TCPserver{
	int server_sock;
	int port;
	struct sockaddr_in server_addr;
	void (*setServer)(struct TCPserver * this,const int _port);
	void (*serve_forever)(struct TCPserver * this);
}TCPserver;

void TCPserver_setServer(struct TCPserver * this,const int _port){
	this -> server_sock = socket(PF_INET,SOCK_STREAM,0);
	this->server_addr.sin_family = AF_INET;
	this->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->server_addr.sin_port = htons(_port);

	if(bind(this->server_sock,(struct sockaddr *)&(this->server_addr),sizeof(this->server_addr)) == -1){
		printf("> bind() failed and program terminated\n");
		close(this->server_sock);
		return;
	}

	if(listen(this->server_sock,5) == -1){
		printf("> listen() failed and program terminated\n");
		close(this->server_sock);
		return;
	}
}

void TCPserver_serve_forever(struct TCPserver * this){
	int client_socket;
	struct sockaddr_in client_addr;
	int client_addr_size;
	int recv_len = 0;
	char buff[BUFFSIZE];
	
	while(1){
		client_addr_size = sizeof(client_addr);
		client_socket = accept(this->server_sock,(struct sockaddr *)&client_addr,&client_addr_size);
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
				break;
			}
		}
	}
}

void TCPserver_init(struct TCPserver * this){
	this->setServer = TCPserver_setServer;
	this->serve_forever = TCPserver_serve_forever;
}



int main(int argc,char ** argv){
	printf("> echo-server is activated\n");
	int port = 65456;
	TCPserver server;

	TCPserver_init(&server);

	server.setServer(&server,port);
	server.serve_forever(&server);

	printf("> echo-server is de-activated\n");
	return 1;
}
