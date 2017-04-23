//client.c
//A client program used to communicate with the server

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "echo_c_functions.h"

int main(int argc, char *argv[])
{
  int sockfd, portno, n, n2, sockfd2;
  unsigned int length;
  struct sockaddr_in serv_addr, from;
  struct hostent *server;

  char buffer[256];
  char buffer2[256];

	argCheck(argc, argv[0]);
	portno = atoi(argv[2]);
  
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	sockfd2 = socket(AF_INET, SOCK_DGRAM, 0);

  //Check socket
  //socketCheck(sockfd, sockfd2);

  //Check server
  server = gethostbyname(argv[1]);
	serverCheck(server);

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  length = sizeof(struct sockaddr_in);

  //Check connection to server
	connectionCheck(sockfd, serv_addr);

  //Process to communicate with server
  //Sending message to server
  printf("Enter the message: ");
  bzero(buffer,256);
  fgets(buffer,255,stdin);

	n = write(sockfd,buffer,strlen(buffer));
	writeCheck(n);
  
  
	n2 = sendto(sockfd2,buffer,strlen(buffer),0,(const struct sockaddr *)&serv_addr,length);
	if (n2 < 0)
		error("ERROR Sendto");
		
  

  //Recieving message from server
  bzero(buffer,256);
 
	n = read(sockfd,buffer,255);
	readCheck(n);
  
  
  n2 = recvfrom(sockfd2,buffer2,256,0,(struct sockaddr *)&from, &length);
  if (n2 < 0)
    error("ERROR recvfrom");
	
  
	printf("Message from TCP: ");  
	printf("%s",buffer);
  	printf("Message from UDP: ");
	printf("%s\n",buffer2);

  //Close sockets before ending program
  
	close(sockfd);
  
	close(sockfd2);
  return 0;
}
