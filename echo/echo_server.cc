// #include <stdio.h>
#include <stdlib.h>
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
  int server_sock;  // 监听socket
  int client_sock;  // 连接socket 

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
  while (true)
  {
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size); // 返回一个新的client的fd
    if (client_sock == -1)
      error_handling("accept() error");
    else
      std::cout << "Connected client: " << ++num << std::endl;

    while((str_len = read(client_sock, message, BUF_SIZE)) != 0) {
      // Todo read and write is puzzling!!
      write(client_sock, message, str_len);
    }
    


    close(client_sock);
  }

  close(server_sock);

  return EXIT_SUCCESS;
}

/*
socket()
bind()
listen(server_sock) 加入未连接队列，等待三次握手成功后，放入连接成功队列
accept(server_sock) 从握手成功的队列取出首元素
read() / write()
close()
*/