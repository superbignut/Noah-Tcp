#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string_view>

#define BUF_SIZE 1024

void error_handling(std::string_view message)
{
    std::cerr << message << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{

    int sock;
    struct sockaddr_in server_addr;

    char message[BUF_SIZE];

    int str_len;

    if (argc != 3)
    {
        std::cout << "Usage ./xxx <IP> <port>\n";
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        error_handling("socket errror");
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;            // IPv4
    inet_aton(argv[1], &server_addr.sin_addr);   // Ip
    server_addr.sin_port = htons(atoi(argv[2])); // port

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) // client的<ip>和<port>自动分配
    {
        error_handling("connect() error");
    }
    else
    {
        std::cout << "Connected..........." << std::endl;
    }

    while (true)
    {
        std::cout << "Input message(Q to quit): " << std::endl;
        fgets(message, BUF_SIZE, stdin); // \n is also included.

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        
        write(sock, message, strlen(message));

        str_len = read(sock, message, BUF_SIZE - 1);

        message[str_len] = 0; // 0-ascii
        std::cout << "Message from server: " << message << std::endl;
    }
    close(sock);

    return EXIT_SUCCESS;
}

/*
socket
connect 第二次握手，发出第三个ACK之后，就成功跳出

*/