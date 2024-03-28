// #include <stdio.h>
#include <stdlib.h>
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
  int server_sock; // 监听socket

  struct sockaddr_in server_addr; //
  struct sockaddr_in client_addr;

  socklen_t client_addr_size; // unsigned int

  char message[BUF_SIZE];

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " "
              << "<port>\n";
    return EXIT_FAILURE;
  }

  server_sock = socket(PF_INET, SOCK_DGRAM, 0); // IPv4 socket STREAM needed to be changed to DGRAM.
  if (server_sock == -1)
    error_handling("socket() error");

  memset(&server_addr, 0, sizeof(server_addr)); // 初始化服务器地址

  server_addr.sin_family = AF_INET;                // IPv4 address
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // in_addr_t uint32_t
  server_addr.sin_port = htons(atoi(argv[1]));     // in_port_t uint16_t

  if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) // 处理服务器socket
    error_handling("bind() error");

  client_addr_size = sizeof(client_addr);

  std::size_t str_len = 0;
  for(int i = 0; i < 3; ++i)
  {
    sleep(5);

    str_len = recvfrom(server_sock, message, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_size);

    // sendto(server_sock, message, str_len, 0, (struct sockaddr *)&client_addr, client_addr_size);

    printf("Message %d: %s \n", i + 1, message);
  }

  close(server_sock); // 这里调用了close之后，剩下的应该就是内核进行接管了

  return EXIT_SUCCESS;
}

/*
socket()
bind()
listen(server_sock) 加入未连接队列，等待三次握手成功后，放入连接成功队列
accept(server_sock) 从握手成功的队列取出首元素
read() / write()  // client close时， read 返回0
close()
*/