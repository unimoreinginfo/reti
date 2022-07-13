#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1024], input[64], address[64];
    bzero(input, 64);
    strcpy(address, argv[1]);
    strcpy(input, argv[2]);

    /*if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }*/

    portno = 8080;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(address);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = 8080;
    if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    
    bzero(buffer, 256);
    // buffer è la stringa da inviare
    // strcat dest, source
    strcpy(buffer, "token from user: ");

    strcat(buffer, input);
    // printf("sending string => %s\n", buffer);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) error("ERROR writing to socket");
    
    printf("spedito token: '%s'\n", input);
    bzero(buffer, 1024);
    n = read(sockfd, buffer, 1024);

    if (n < 0) error("ERROR reading from socket");

    // ("received => %s\n", buffer);
    const char separator[2] = ":";
    char* actual_message = malloc(sizeof(char) * 1024);
    bzero(actual_message, 1024);
    char* chunk = strtok(buffer, separator);
    chunk = strtok(NULL, separator); // sul server è spiegato l'utilizzo di strtok

    printf("ricevuto token:'%s'\n", chunk);

    close(sockfd);
    
}