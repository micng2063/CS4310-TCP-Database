#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

struct student {
   int id;
   char fName[20];
};

void connectSocket(int newSocket){ 	// communication starts from here
	uint32_t num;
	char msg[30];
	// receive an integer from the client
	recv(newSocket, &num, sizeof(num), 0);
	printf("# received: %d\n",ntohl(num));   
	
	recv(newSocket, msg, sizeof(msg), 0);
	printf("%s\n", msg);
	
	struct student stuData[20];
	stuData[0].id = ntohl(num); // if not set ntohl, num = address
	strcpy(stuData[0].fName, msg);
	
	printf("ID: %d ", stuData[0].id);
	printf("Name: %s ", stuData[0].fName);
	
	/*	// send a reply message to the client
	strcpy(msg, "ID received");
	send(newSocket, msg, sizeof(msg), 0);*/
	
}

int main(int argc, char **argv){
  int welcomeSocket, newSocket;
  char buffer[1024];
/* sockaddr_in is an IPv4-specific address structure used for storing internet addresses. */
  struct sockaddr_in serverAddr;
/* sockaddr_storage is a generic address structure used for storing addresses of various types, such as IPv4 and IPv6. */
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;
  unsigned short port; /* port server binds to */


  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;

  /* Set port number, using htons function to use proper byte order */
  port = (unsigned short) atoi(argv[1]);
  serverAddr.sin_port = htons(port);

  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  /*---- Accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
	connectSocket(newSocket);
  
  close(newSocket);
  close(welcomeSocket);

  return 0;
}
