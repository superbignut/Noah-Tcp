#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string_view>

#define BUF_SIZE 30

void error_handling(std::string_view message)
{
    std::cerr << message << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{

    int sock;
    struct sockaddr_in server_addr, from_adr;

    char msg1[] = "Hi!";
    char msg2[] = "I'm another UDP";
    char msg3[] = "Nice to meet";
    char message[BUF_SIZE];

    if (argc != 3)
    {
        std::cout << "Usage ./xxx <IP> <port>\n";
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);

    if (sock == -1)
    {
        error_handling("socket errror");
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;            // IPv4
    inet_aton(argv[1], &server_addr.sin_addr);   // Ip
    server_addr.sin_port = htons(atoi(argv[2])); // port

    int str_len;

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)); // 如果不是每次都变更目标地址，也可以使用connect进行提前连接，省去了分配地址的过程

/*     sendto(sock, msg1, strlen(msg1), 0, (sockaddr *)&server_addr, sizeof(server_addr)); // UDP是具有数据边界的协议，传输中调用I/O函数的此书非常重要

    sendto(sock, msg2, strlen(msg2), 0, (sockaddr *)&server_addr, sizeof(server_addr));

    sendto(sock, msg3, strlen(msg3), 0, (sockaddr *)&server_addr, sizeof(server_addr)); */

    int receive_len, receive_cnt;
    while (true)
    {
        std::cout << "Input message(Q to quit): " << std::endl;
        fgets(message, BUF_SIZE, stdin); // \n is also included. 当让最后还有一个 null character.

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        str_len = write(sock, message, strlen(message));

        receive_len = read(sock, message, BUF_SIZE - 1);

        message[str_len] = 0; // 0-ascii
        std::cout << "Message from server: " << message << std::endl;
    }
    close(sock);
    // 可以理解这个close为单向的关闭，发送完缓冲区数据后，发送EOF，即进行第一次挥手，服务器受到后，内核自动ack了，并返回一个read的 EOF，让用户态知道，
    // 然后直到服务器端close 时，开始第三次挥手，客户端这边进行第四次ack， 当然这些内容都是内核中进行的。

    return EXIT_SUCCESS;
}

/*
socket
connect 第二次握手，发出第三个ACK之后，就成功跳出

潜在问题：

每次 read 和 write 都会以字符串为单位执行实际的IO操作
*/