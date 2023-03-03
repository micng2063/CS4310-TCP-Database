#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
void whileLoop (int newSocket){
	recv(newSocket, &num, sizeof(num), 0);
	printf("Integer received: %d\n",ntohl(num));   
	
	strcpy(msg, "Integer received");
	send(newSocket, msg, sizeof(msg), 0);
}

int main(int argc, char **argv){
	int welcomeSocket, newSocket;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	unsigned short port; 
	uint32_t num;
	char msg[30];
	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	port = (unsigned short) atoi(argv[1]);
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	if(listen(welcomeSocket,5)==0)
		printf("Listening\n");
	else
		printf("Error\n");
	addr_size = sizeof serverStorage;
	newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
	
	whileLoop(newSocket);
	
	close(newSocket);
	close(welcomeSocket);
	return 0;
}
