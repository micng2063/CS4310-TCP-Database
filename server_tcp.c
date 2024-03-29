#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>

struct student {
   int id;
   char fName[20];
   char lName[20];
   int score;
};

void connectSocket(int newSocket){
	uint32_t num;
	char msg[30];
	
	recv(newSocket, &num, sizeof(num), 0);
	
	struct student stuData[20] = {	{ 123 , "Michelle" , "McMichelle", 90 } , 
								{ 133 , "Michael" , "McMichael", 90 } ,   
								{ 143 , "Mitchell" , "McMitchel", 89 }	};
	int i = 3; // size
	
	while (ntohl(num) != 6){
		if (ntohl(num) == 1){
			uint32_t numID;
			uint32_t score;
			char fName[30];
			char lName[30];
			
			recv(newSocket, &numID, sizeof(numID), 0);
			recv(newSocket, fName, sizeof(fName), 0);
			recv(newSocket, lName, sizeof(lName), 0);
			recv(newSocket, &score, sizeof(score), 0);
			
			stuData[i].id = ntohl(numID); // if not set ntohl, num = address
			strcpy(stuData[i].fName, fName);
			strcpy(stuData[i].lName, lName);
			stuData[i].score = ntohl(score); 
			i++;
			
		}	
		else if (ntohl(num) == 2){
			uint32_t searchID;
			recv(newSocket, &searchID, sizeof(searchID), 0);
			bool foundID = false;
			
			char msgSearch[50];
			int j; 
			for (j = 0; j < i; j++){
				if (stuData[j].id == ntohl(searchID))
				{
					foundID = true;
					break;
				}
			}
			if (foundID == true){
				strcpy(msgSearch, stuData[j].fName);
				strcat(msgSearch, " ");
				strcat(msgSearch, stuData[j].lName);
				strcat(msgSearch, " is found with the matching ID.");
			}
			else{
				strcpy(msgSearch, "ID cannot be found in database.");
			}
			
			send(newSocket, msgSearch, sizeof(msgSearch), 0);
				
		}
		else if (ntohl(num) == 3){
			
			uint32_t searchScore;
			recv(newSocket, &searchScore, sizeof(searchScore), 0);
			
			int j;
			int k = 0;
			char msgScore[100];
			strcpy(msgScore, "Student with this score: ");
			for (j = 0; j < i; j++){
				if (stuData[j].score == ntohl(searchScore))	{
					strcat(msgScore, stuData[j].fName);
					strcat(msgScore, stuData[j].lName);
					strcat(msgScore, "\t");
					k++; 
				}
			}
			if (k == 0){
				strcpy(msgScore, "No students with this score can be found.");
			}
			send(newSocket, msgScore, sizeof(msgScore), 0);
			
		}
		else if(ntohl(num) == 4){
			int j;
			
			uint32_t size, cSize;
			size = i;
			cSize = htonl(size); 
			send(newSocket, &cSize, sizeof(cSize), 0);
		
			for (j = 0; j < i; j++){
				uint32_t id, cID;
				uint32_t score, cScore;
				char msgfName[50];
				char msglName[50];
				
				strcpy(msgfName, stuData[j].fName);
				strcpy(msglName, stuData[j].lName);
				id = stuData[j].id;
				cID = htonl(id); 
				score = stuData[j].score;
				cScore = htonl(score);
				
				send(newSocket, &cID, sizeof(cID), 0);
				send(newSocket, msgfName, sizeof(msgfName), 0);
				send(newSocket, msgfName, sizeof(msglName), 0);
				send(newSocket, &cScore, sizeof(cScore), 0);
			}
		}
		else if (ntohl(num) == 5){
			uint32_t deleteID;
			recv(newSocket, &deleteID, sizeof(deleteID), 0);
			bool foundID = false;
			
			int j;
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
					stuData[k].score = stuData[k+1].score;
					strcpy(stuData[k].fName, stuData[k+1].fName );
					strcpy(stuData[k].lName, stuData[k+1].lName );
				}
				i--;
			}
		}
		
		recv(newSocket, &num, sizeof(num), 0);
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

  close(newSocket);
  close(welcomeSocket);
  return 0;
}
