#include "http.h"

void *get_in_addr(struct sockaddr *sa)
{
    if(sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void sig_handler(int signo)
{
    printf("caught sigint\n");
    exit(EXIT_SUCCESS);
}