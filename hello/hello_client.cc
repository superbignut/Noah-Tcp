#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string_view>

void error_handling(std::string_view message)
{
    std::cerr << message << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{

    int sock;
    struct sockaddr_in server_addr;

    char message[30];

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

    server_addr.sin_family = AF_INET;
    inet_aton(argv[1], &server_addr.sin_addr);
    server_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Error");
        error_handling("connect() error");
    }

    str_len = read(sock, message, sizeof(message) - 1); // 为什么-1
    std::cout << str_len << std::endl;
    if (str_len == -1)
        error_handling("read() error");

    std::cout << "Message from server is :" << message << std::endl;
    // 可以write数据
    close(sock);

    return EXIT_SUCCESS;
}