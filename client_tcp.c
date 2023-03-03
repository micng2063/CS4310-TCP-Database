#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdbool.h>

void showMenu(int clientSocket);
void insertEntry(int clientSocket);
void searchID(int clientSocket);
void deleteID(int clientSocket);
void display(int clientSocket);

void connectSocket(int clientSocket){
	showMenu(clientSocket);
}

void showMenu(int clientSocket){
	
	printf( "-------------------------\n");
	printf( "- Student Database Menu -\n");
	printf( "-------------------------\n");
	printf( "1. Add Entry\n");
	printf( "2. Search with ID\n");
	printf( "3. Search with Score\n");
	printf( "4. Display Database\n");
	printf( "5. Delete Entry\n");
	printf( "6. Exit\n");
	printf( "-------------------------\n");
	printf( "Enter choice: ");
	
	uint32_t num, cnum; // communication starts from here
	char msg[30];
	scanf("%d", &num);
	cnum = htonl(num); 
	send(clientSocket, &cnum, sizeof(cnum), 0);
	
	recv(clientSocket, msg, sizeof(msg), 0);
	printf("%s\n", msg);
	
	if(num == 1){
		uint32_t numID, cID; // communication starts from here
		char fName[30];
		printf( "Enter student ID: ");
		scanf("%d", &numID);
		cID = htonl(numID); // "host to network long" convert values from host byte order to network byte order
		send(clientSocket, &cID, sizeof(cID), 0);
		
		printf( "Enter student name: ");
		scanf("%s", &fName);
		send(clientSocket, fName, sizeof(fName), 0);
			
		char msgInsert[30];
		recv(clientSocket, msgInsert, sizeof(msgInsert), 0);
		printf("%s\n", msgInsert);
	}
	else if (num == 2){
		uint32_t searchID, csearchID;
		printf( "Enter ID for searching: ");
		scanf("%d", &searchID);
		csearchID = htonl(searchID);
		send(clientSocket, &csearchID, sizeof(csearchID), 0);
		
		char msgSearch[50];
		recv(clientSocket, msgSearch, sizeof(msgSearch), 0);
		printf("%s\n", msgSearch);
	}
	else if (num == 4){
		uint32_t size;
		recv(clientSocket, &size, sizeof(size), 0);
		//printf("Size received: %d\n",ntohl(size));   
		printf( "-------------------------\n");
		int j;
		for (j = 0; j < ntohl(size); j++){
			uint32_t id;
			char msgfName[50];
			int order = j + 1;
			
			recv(clientSocket, &id, sizeof(id), 0);
			recv(clientSocket, msgfName, sizeof(msgfName), 0);
			printf("%d .", order);
			printf("%s \t", msgfName);
			printf(" - %d\n",ntohl(id));   
		}
		printf( "-------------------------\n");
	}
	else if (num == 5){
		uint32_t deleteID, cdeleteID;
		printf( "Enter ID for deletion: ");
		scanf("%d", &deleteID);
		cdeleteID = htonl(deleteID);
		send(clientSocket, &cdeleteID, sizeof(cdeleteID), 0);
	}
	else
	{
		printf("Input not valid. Please enter choice again...\n");
		showMenu(clientSocket);
	}
	
}

void insertEntry(int clientSocket){
	uint32_t numID, cID; // communication starts from here
	char fName[30];
	printf( "Enter student ID: ");
	scanf("%d", &numID);
	cID = htonl(numID); // "host to network long" convert values from host byte order to network byte order
	send(clientSocket, &cID, sizeof(cID), 0);
	
	printf( "Enter student name: ");
	scanf("%s", &fName);
	send(clientSocket, fName, sizeof(fName), 0);
		
	char msgInsert[30];
	recv(clientSocket, msgInsert, sizeof(msgInsert), 0);
	printf("%s\n", msgInsert);
}

void searchID(int clientSocket){
	uint32_t searchID, csearchID;
	printf( "Enter ID for searching: ");
	scanf("%d", &searchID);
	csearchID = htonl(searchID);
	send(clientSocket, &csearchID, sizeof(csearchID), 0);
	
}

void deleteID(int clientSocket){
	uint32_t deleteID, cdeleteID;
	printf( "Enter ID for deletion: ");
	scanf("%d", &deleteID);
	cdeleteID = htonl(deleteID);
	send(clientSocket, &cdeleteID, sizeof(cdeleteID), 0);
}

void display(int clientSocket){
	printf("Displaying database...\n");
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

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;

  /* set port number*/ 
  port = (unsigned short)atoi(argv[2]);

  /* htons() stands for "host to network short" and is used to convert the port number stored in the "port" variable from host byte order to network byte order, ensuring that data is not misinterpreted when sent over the network. */
  serverAddr.sin_port = htons(port);

  /* Set IP address to localhost */
  hostnm = gethostbyname(argv[1]);

/* This sets the serverAddr structure's sin_addr member to the host address provided by the hostnm->h_addr variable. The s_addr field contains the IP address of the host in network byte order, and the *((unsigned long *)hostnm->h_addr) expression casts the data to an unsigned long type. This ensures that the IP address taken from hostnm is formatted correctly for network communication. */
  serverAddr.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr); 

/* Set all bits of the padding field to 0. It is used to ensure that the serverAddr structure is correctly zero initialized before use, which is necessary for certain network operations. */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
 	connectSocket(clientSocket);

		close(clientSocket);
  return 0;
}

