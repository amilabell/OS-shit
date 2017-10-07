#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_un serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr, "ERROR, no socket file provided\n");
         exit(1);
     }
     sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
     if (sockfd < 0) 
         error("ERROR opening socket");
     memset(&serv_addr, 0, sizeof(serv_addr));
     serv_addr.sun_family = AF_UNIX;
     strcpy(serv_addr.sun_path, argv[1]);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
         error("ERROR on binding");
     listen(sockfd, 5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
         error("ERROR on accept");
     bzero(buffer, 256);
     n = read(newsockfd, buffer, 255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n", buffer);
     n = write(newsockfd, "I got your message, mate ;)", 27);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
     return 0; 
}
