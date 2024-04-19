#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string_view>

#define BUF_SIZE 1024

#define RLT_SIZE 4

#define OPSZ 4

void error_handling(std::string_view message)
{
    std::cerr << message << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server_addr;

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
        std::cout << "Connected Successfully..........." << std::endl;
    }

    fputs("Operand count: ", stdout);

    int operand_cnt, str_len, receive_len, receive_cnt, result;

    char msg[BUF_SIZE];

    scanf("%d", &operand_cnt); // 读取一个数字

    msg[0] = char(operand_cnt);

    for (int i = 0; i < operand_cnt; ++i)
    {
        printf("Oprand %d : ", i + 1);

        scanf("%d", (int *)&msg[i * OPSZ + 1]);
    }
    fgetc(stdin); // 吃掉回车

    fputs("Operator: ", stdout);

    scanf("%c", &msg[operand_cnt * OPSZ + 1]);

    write(sock, msg, operand_cnt * OPSZ + 2);

    read(sock, &result, RLT_SIZE);

    printf("Result is : %d\n", result);

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