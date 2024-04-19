#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>

#define BUF_SIZE 30

void error_handling(std::string_view message)
{
    std::cerr << message << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{

    int i;
    struct hostent *host;

    struct sockaddr_in addr;

    if (argc != 2)
    {
        std::cout << "Usage ./xxx <addr>\n";
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));

    inet_aton(argv[1], &(addr.sin_addr));

    host = gethostbyaddr((void *)&addr.sin_addr, 4, AF_INET);
    
    if (host == nullptr)
    {
        error_handling("get host error.");
    }
    printf("offical name: %s \n", host->h_name);

    for (int i = 0; host->h_aliases[i] != nullptr; ++i)
    {
        printf("Aliases %d: %s \n", i, host->h_aliases[i]);
    }
    printf("Address type: %d \n", host->h_addrtype);

    for (int i = 0; host->h_aliases[i] != nullptr; ++i)
    {
        printf("Aliases %d: %s \n", i, inet_ntoa(*((struct in_addr *)host->h_addr_list[i])));
    }
}