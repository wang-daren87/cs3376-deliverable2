//server.c
/*A simple server in the internet domain using TCP.
This version runs forever, forking off a separate
process for each connection*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include "echo_s_functions.h"

int main(int argc, char *argv[])
{
	int sockfd, sockfd2, length, newsockfd, portno, pid, pid2, sockL;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	struct sockaddr_in log_addr;

	//check arguments if there is a port provided
	chkArgument(argc, argv[0]);
	sockL = socket(AF_INET, SOCK_DGRAM, 0);
	//sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//sockfd2 = socket(AF_INET, SOCK_DGRAM, 0);

	//check if the socket has been opened
	//chkSock(sockfd, sockfd2);
	length = sizeof(serv_addr);
	
	bzero((char *) &log_addr, sizeof(log_addr));
	log_addr.sin_family = AF_INET;
	log_addr.sin_addr.s_addr = INADDR_ANY;
	log_addr.sin_port = htons(57171);

	if (connect(sockL,(struct sockaddr *)&log_addr,sizeof(log_addr)) < 0)
  		error("ERROR connecting");
	//chkBind2(sockL, serv_addr, length);
	
	pid = fork();
	if(pid != 0){
		pid2 = fork();
	}
	if (argc >=3 && pid ==0){
		
		
		portno = atoi(argv[2]);
		
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(portno);
		
	
		
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			chkBind1(sockfd, serv_addr);
		
		
			sockfd2 = socket(AF_INET, SOCK_DGRAM, 0);
			chkBind2(sockfd2, serv_addr, length);
		
		
		
	}else if(argc < 3 && pid ==0){
		exit(0);
	}else if(argc == 4 && pid2 ==0){
		
		bzero((char *) &serv_addr, sizeof(serv_addr));
			portno = atoi(argv[3]);
		
		serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
        
		
		
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			chkBind1(sockfd, serv_addr);
		
		
			sockfd2 = socket(AF_INET, SOCK_DGRAM, 0);
			chkBind2(sockfd2, serv_addr, length);
		
        
        
	}else if(argc < 4 && pid2==0){
		exit(0);
	}else{
		
		bzero((char *) &serv_addr, sizeof(serv_addr));
		
			portno = atoi(argv[1]);
		
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(portno);
		
		
		
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			chkBind1(sockfd, serv_addr);
		
		
			sockfd2 = socket(AF_INET, SOCK_DGRAM, 0);
			chkBind2(sockfd2, serv_addr, length);
		
		
		
	}

	//check if server is binded
	//chkBind(sockfd, serv_addr,sockfd2, length);

	//look for a connection
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	//forking off processes to handle new connections continuously
	while (1)
	{
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");
		pid = fork();
		if (pid < 0)
			error("ERROR on fork");
		if (pid == 0)
		{
			close(sockfd);
			dostuff(newsockfd, cli_addr, clilen, sockfd2, sockL, log_addr);
			close(sockfd);
			close(sockfd2);
			close(sockL);
			exit(0);
		}
		else close(newsockfd);
		signal(SIGCHLD,SIG_IGN);
	}

	//Close sockets before ending program
	close(sockfd);
	close(sockfd2);

	return 0;
}
