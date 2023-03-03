#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
void whileLoop(int clientSocket){
	printf("Enter an integer: ");
	scanf("%d", &num);
	
	cnum = htonl(num);
	send(clientSocket, &cnum, sizeof(cnum), 0);
	
	recv(clientSocket, msg, sizeof(msg), 0);
	printf("%s\n", msg);
}
int main(int argc, char **argv){
	int clientSocket;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	unsigned short port;
	struct hostent *hostnm;
	uint32_t num, cnum;
	char msg[30];
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	
	serverAddr.sin_family = AF_INET;
	port = (unsigned short)atoi(argv[2]);
	serverAddr.sin_port = htons(port);
	hostnm = gethostbyname(argv[1]);
	serverAddr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr); 
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
	addr_size = sizeof serverAddr;
	connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
	// communication starts from here
	// send an integer to the server
	whileLoop(clientSocket);
	close(clientSocket);
	return 0;
}
