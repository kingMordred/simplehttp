#include "http.h"

int main(int argc, char *argv[])
{
    int status, sockfd, new_fd;
    struct addrinfo hints, *res;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if((status = getaddrinfo(NULL, "8080", &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo erro: %s\n", gai_strerror(status));
        exit(1);
    }

    if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        perror("server socket");
        exit(1);
    }

    if(bind(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        close(sockfd);
        perror("bind");
        exit(1);
    }

    freeaddrinfo(res);

    if(listen(sockfd, 10) == -1) //backlog is 10 i.e pending connections queue size
    {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1)
    {
        addr_size = sizeof(client_addr);
        new_fd = accept(sockfd, (struct sockaddr *) &client_addr, &addr_size);
        if(new_fd == -1)
        {
            perror("new fd");
            exit(1);
        }
        printf("connection accepted for now\n");
    }


    
    
}