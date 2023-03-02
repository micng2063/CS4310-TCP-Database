#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>

struct student {
   int id;
   char fName[20];
};

void insertEntry(int newSocket, struct student stu);
void searchID(int newSocket, struct student stu);
void deleteID(int newSocket, struct student stu);
void display(int newSocket, struct student stu);

void connectSocket(int newSocket){
	uint32_t num;
	char msg[30];
	
	recv(newSocket, &num, sizeof(num), 0);
	printf("Choice received: %d\n",ntohl(num));   
	
	if (ntohl(num) == 1)	strcpy(msg, "Inserting entry...\n");
	else if (ntohl(num) == 2) 	strcpy(msg, "Searching by ID...\n");
	else if (ntohl(num) == 3) 	strcpy(msg, "Searching by score...\n");
	else if (ntohl(num) == 4) 	strcpy(msg, "Displaying student database...\n");
	else if (ntohl(num) == 5) 	strcpy(msg, "Deleteing entry...\n");
	else if (ntohl(num) == 6)	strcpy(msg, "Quitting... \n");
	
	send(newSocket, msg, sizeof(msg), 0);
	
	struct student stuData[20];
	int i = 0;
	
	strcpy(stuData[0].fName, "Michelle");	
	stuData[0].id = 123;
	i++;
	strcpy(stuData[1].fName, "Mitchel");
	stuData[1].id = 133;
	i++;
	strcpy(stuData[2].fName, "Michael");
	stuData[2].id = 143;
	i++;
	
	if (ntohl(num) == 1){
		insertEntry(newSocket, stuData);
		i++;
	}	
	else if (ntohl(num) == 2){
		searchID(newSocket, stuData);
	}
	else if(ntohl(num) == 4, stuData){
		display(newSocket, stuData);
	}
	else if (ntohl(num) == 5){
		deleteEntry(newSocket, stuData);
	}
	
}

void insertEntry(int newSocket, struct student stu){
	uint32_t numID;
	char fName[30];
	
	recv(newSocket, &numID, sizeof(numID), 0);
	recv(newSocket, fName, sizeof(fName), 0);
	
	stuData[i].id = ntohl(numID); // if not set ntohl, num = address
	strcpy(stuData[i].fName, fName);

}

void searchID(int newSocket, struct student stu){
	uint32_t searchID;
	recv(newSocket, &searchID, sizeof(searchID), 0);
	printf("SearchID received: %d\n",ntohl(searchID)); 
	bool foundID = false;
	
	int j; // SEARCH ID
	for (j = 0; j < i; j++){
		if (stuData[j].id == ntohl(searchID))
		{
			foundID = true;
			break;
		}
	}
	if (foundID == true){
		printf("ID found with name: %s \n", stuData[j].fName);
	}
	else
		printf("ID not found in database.\n");
}

void deleteID(int newSocket, struct student stu){
	uint32_t deleteID;
		recv(newSocket, &deleteID, sizeof(deleteID), 0);
		printf("SearchID received: %d\n",ntohl(deleteID)); 
		bool foundID = false; // search ID for deletion
		
		int j; // SEARCH ID
		int k;
		for (j = 0; j < i; j++){
			if (stuData[j].id == ntohl(deleteID))
			{
				foundID = true;
				break;
			}
		}
		
		if (foundID == true){
			for (k = j; k < i; k++){
				char fNameT[30];
				
				stuData[k].id = stuData[k+1].id;
				strcpy(stuData[k].fName, stuData[k+1].fName );
			}
			i--;
		}
		else
			printf("ID not found in database.\n");
}
void display(int newSocket, struct student stu){
	int j;
	for (j = 0; j < i; j++){
		printf("%d .", j);
		printf("Name: %s \t", stuData[j].fName);
		printf("ID: %d \t", stuData[j].id);
		printf("\n");
	}
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

	close(welcomeSocket);
  return 0;
}
