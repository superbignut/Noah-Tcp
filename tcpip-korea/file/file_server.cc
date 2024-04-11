#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#define BUF_SIZE 30

void error_handling(std::string_view message)
{
  std::cerr << message << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  int server_sock; // 监听socket
  int client_sock; // 连接socket

  FILE *fp;

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

  fp = fopen("file_server.cc", "rb"); /// 只读打开文件

  server_sock = socket(PF_INET, SOCK_STREAM, 0); // IPv4 socket

  if (server_sock == -1)
    error_handling("socket() error");

  memset(&server_addr, 0, sizeof(server_addr)); // 初始化服务器地址

  server_addr.sin_family = AF_INET;                // IPv4 address
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // in_addr_t uint32_t
  server_addr.sin_port = htons(atoi(argv[1]));     // in_port_t uint16_t

  if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) // 处理服务器socket
    error_handling("bind() error");

  if (listen(server_sock, 5) == -1) // 转换为被动socket，如果有相应的syn到达，则添加到监听队列，并开始三次握手
    error_handling("Listen() error");

  client_addr_size = sizeof(client_addr);

  std::size_t num = 0;
  std::size_t str_len = 0;

  client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size); // 返回一个新的client的fd

  int read_cnt;

  while (true)
  {
    read_cnt = fread((void *)message, 1, BUF_SIZE, fp); // 最多读 30 个字节
    if (read_cnt < BUF_SIZE)
    {
      write(client_sock, message, read_cnt);
      break;
    }
    write(client_sock, message, BUF_SIZE);
  }
  shutdown(client_sock, SHUT_WR);

  int read_len = read(client_sock, message, BUF_SIZE);
  // message[read_len] = 0;

  printf("Message from client : %s\n", message);

  fclose(fp);
  close(client_sock);
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