#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string_view>
#include <wait.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handling(std::string_view message)
{
  std::cerr << message << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  int server_sock; // 监听socket
  int client_sock; // 连接socket

  struct sockaddr_in server_addr; //
  struct sockaddr_in client_addr;

  struct timeval tiemout;

  fd_set reads, cpy_reads;

  socklen_t client_addr_size; // unsigned int

  char message[BUF_SIZE];

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " "
              << "<port>\n";
    return EXIT_FAILURE;
  }

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

  int fd_max, str_len, fd_num;
  FD_ZERO(&reads);
  FD_SET(server_sock, &reads);
  fd_max = server_sock; // important!!!

  while (true)
  {
    cpy_reads = reads;
    tiemout.tv_sec = 5;
    tiemout.tv_usec = 5000;

    if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &tiemout)) == -1) // 所以只有这里在阻塞， 并判断超时
    {
      error_handling("select error!");
    }
    if (fd_num == 0)
      continue; // timeout

    for (int i = 0; i < fd_max + 1; ++i)
    {
      if (FD_ISSET(i, &cpy_reads))// 有完成的新连接， 也会触发，进而去不阻塞的accept，
      { 
        if (i == server_sock)
        {
          client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size); // 正在阻塞的信号会被挂起，然后去处理信号去
          FD_SET(client_sock, &reads);
          if (fd_max < client_sock)
            fd_max = client_sock;
          std::cout << "Connected client, socket is:" << client_sock << std::endl;
        }
        else
        { // 不是server的accept触发，就是数据到了触发， 或者EOF触发
          str_len = read(i, message, BUF_SIZE);
          if (str_len == 0)
          { // close request
            FD_CLR(i, &reads);
            close(i);
          }
          else
            write(i, message, str_len); // echo
        }
      }
    }
  }

  close(server_sock); // 这里调用了close之后，剩下的应该就是内核进行接管了

  return EXIT_SUCCESS;
}

/*s intended as a hub of links, documentation and resources related to the project.*/