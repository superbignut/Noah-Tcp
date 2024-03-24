// #include <stdio.h>
#include <stdlib.h>
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
  int server_sock; // 两个socket
  int client_sock;

  struct sockaddr_in server_addr; // 
  struct sockaddr_in client_addr;

  socklen_t client_addr_size; // unsigned int

  char message[] = "Hello World!";

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " "
              << "<port>\n";
    return EXIT_FAILURE;
  }

  server_sock = socket(PF_INET, SOCK_STREAM, 0);    // IPv4 socket

  if (server_sock == -1)
    error_handling("socket() error");

  memset(&server_addr, 0, sizeof(server_addr));     // 初始化服务器地址

  server_addr.sin_family = AF_INET;                 // IPv4 address
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // in_addr_t uint32_t
  server_addr.sin_port = htons(atoi(argv[1]));      // in_port_t uint16_t

  if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) // 处理服务器socket
    error_handling("bind() error");

  if (listen(server_sock, 5) == -1)
    error_handling("Listen() error");

  client_addr_size = sizeof(client_addr);

  client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size); // 阻塞
  if (client_sock == -1)
    error_handling("accept() error");

  write(client_sock, message, sizeof(message)); // 传输数据

  close(client_sock);
  // 可以读到数据
  close(server_sock);

  return EXIT_SUCCESS;
}
