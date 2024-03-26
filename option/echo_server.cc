// #include <stdio.h>
#include <stdlib.h>
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

int calculate(int opnum, int opnds[], char op)
{
  int sum = 0;
  for (int i = 0; i < opnum; ++i)
  {
    if (op == '+')
      sum += opnds[i];
    if (op == '-')
      sum -= opnds[i];
    if (op == '*')
      sum *= opnds[i];

    if (op == '/')
      sum /= opnds[i];
  }
  return sum;
}

int main(int argc, char *argv[])
{
  int server_sock; // 监听socket
  int client_sock; // 连接socket

  struct sockaddr_in server_addr; //
  struct sockaddr_in client_addr;

  socklen_t client_addr_size; // unsigned int

  char msg[BUF_SIZE];

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

  int operand_cnt = 0, num = 0;

  while (true)
  {
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size); // 返回一个新的client的fd
    if (client_sock == -1)
      error_handling("accept() error");
    else
      std::cout << "Connected client: " << ++num << std::endl;

    read(client_sock, &operand_cnt, 1); // 读取 输入 运算数字的个数

    int receive_len = 0, receive_cnt = 0;

    while (receive_len < operand_cnt * OPSZ + 1)
    {
      receive_cnt = read(client_sock, &msg[receive_len], BUF_SIZE - 1); // 读取运算数字的个数和运算符号
      receive_len += receive_cnt;
    }

    int result = calculate(operand_cnt, (int *)msg, msg[operand_cnt * OPSZ]);

    write(client_sock, (char *)&result, sizeof(result));

    close(client_sock);
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
I/O缓冲在每个TCP socket 中单独存在
I/O缓冲在创建 socket 时自动生成 
即使关闭 socket 也会继续传输缓冲中的数据，但是会丢失输入缓冲中的数据
*/