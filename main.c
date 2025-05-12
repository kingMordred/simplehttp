/*
    The goal is a simple http server that can serve a .html file

    Learning source: https://beej.us/guide/bgnet/html/#intro
    getaddrinfo -> socket -> bind -> listen -> accept -> fork -> send/recv
*/

#include "http.h"

int main(int argc, char *argv[])
{
    int status, sockfd, new_fd, yes = 1;
    struct addrinfo hints, *res;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    char s[INET6_ADDRSTRLEN], buf[MAX_BUFF_LEN];

    if(signal(SIGINT, sig_handler) == SIG_ERR)
    {
        perror("cannot handle sigint");
        exit(EXIT_FAILURE);
    }

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

    //reuse a previously bound port
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
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
        inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*) &client_addr), s, sizeof(s));


        printf("connection accepted from: %s\n", s);

        //fork and read/send
        if(!fork())
        {
            close(sockfd);

            if(send(new_fd, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nHello, world!", MAX_BUFF_LEN, 0) == -1)
            {
                perror("send");
            }
            close(new_fd);
            exit(0);
        }
        close(new_fd);
    }


    return EXIT_SUCCESS;
  
}