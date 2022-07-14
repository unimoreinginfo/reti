#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg) {
    // perror ragiona in base alla variabile globale che viene settata 
    // ogni volta che si verifica un errore viene settata questa variabile ERRNO
    perror(msg); 
    exit(1);
}
int main(int argc, char** argv) {

    int socket_fd, port, cli_len, n, new_sock_fd;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[1024];

    /*if (argc < 2) {
        fprintf(stderr, "no port provided\n");
        exit(1);
    }*/
    
    socket_fd = socket(AF_INET, SOCK_STREAM, 0); // definisco un socket di tipo IPv4 (AF_INET) e usa TCP (SOCK_STREAM) con il campo protocol a 0
    if(socket_fd < 0){
        error("error while opening socket\n");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr)); // azzero tutti i byte di sta roba perché contengono dati a caso // parso il numero di porta
    serv_addr.sin_family = AF_INET; // l'ip è ipv4
    serv_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0 mappo su tutte le interfacce sta roba
    serv_addr.sin_port = 8080; // htons => mette la endian-ness del numero di porta allo standard "host-to-network", 
    // in modo tale che se l'host usa una endian-ness diversa da quella del network viene standardizzata (s => short)
    if (bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("error on binding");
    
    listen(socket_fd, 5); // 5 è il limite massimo di connessioni accodate
    cli_len = sizeof(cli_addr);

    while((new_sock_fd = accept(socket_fd, (struct sockaddr *) &cli_addr, &cli_len))){ // loop sequenziale

        if (new_sock_fd < 0)
            error("error on accept");

        bzero(buffer, 1024);

        n = read(new_sock_fd, buffer, 1024);
        if (n < 0) error("error reading from socket");
        
        // printf("read => %s\n", buffer);
        const char separator[2] = ":";
        char* actual_message = malloc(sizeof(char) * 1024);
        bzero(actual_message, 1024);
        char* chunk = strtok(buffer, separator); 
        /* 
            strtok permette
            di ottenere parti di una stringa dato un delimitatore
            https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm

            alternativamente si poteva fare con sprintf/sscanf
        */
        chunk = strtok(NULL, separator);
        // printf("parsed => %s\n", chunk);
        int len = strlen(chunk);
        int i = 0;
        for(; i < len; i++){
            if(chunk[i] >= 'a' && chunk[i] <= 'z') {
                chunk[i] = chunk[i] - 32; 
                /*
                    sottraendo 32 ad una lettera minuscola (un char, quindi)
                    si ottiene la sua corrispondente maiuscola
                    (il codice ascii è strutturato così)
                */
            }
        }

        char send_buff[1024]; // il buffer è scelto grande arbitrariamente
        bzero(send_buff, 1024);
        strcat(send_buff, "token from client:");
        strcat(send_buff, chunk);
        
        n = write(new_sock_fd, send_buff, 1024);
        if (n < 0) error("error writing to socket");
        
    }
    
    return 0;

}
