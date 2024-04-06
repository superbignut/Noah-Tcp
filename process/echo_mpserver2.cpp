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

#define BUF_SIZE 30

void error_handling(std::string_view message)
{
  std::cerr << message << std::endl;
  exit(1);
}
//每当一个进程结束时，操作系统需要保留该进程的一些基本信息，比如退出状态，以便父进程可以查询。
//这个信息对于父进程来说是很重要的，因为它告诉父进程它的子进程是成功完成了任务还是遇到了错误。
void read_childproc(int sig) // 处理子进程的回调函数
{
  pid_t pid;
  int status;
  pid = waitpid(-1, &status, WNOHANG); // 不阻塞
  printf("removed proc id: %d \n", pid);
}

int main(int argc, char *argv[])
{
  int server_sock; // 监听socket
  int client_sock; // 连接socket

  struct sockaddr_in server_addr; //
  struct sockaddr_in client_addr;

  int fds[2];

  pid_t pid;

  int str_len, state;

  struct sigaction act;

  socklen_t client_addr_size; // unsigned int

  char message[BUF_SIZE];

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " "
              << "<port>\n";
    return EXIT_FAILURE;
  }

  act.sa_handler = read_childproc; // 定义回调函数
  sigemptyset(&act.sa_mask);       // 清空mask， 暂时不用
  act.sa_flags = 0;                // 暂时不用

  state = sigaction(SIGCHLD, &act, 0); // 将回调函数绑定到 子进程结束的事件上

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

  std::size_t num = 0; // 统计这是第几个client

  pipe(fds);

  pid = fork(); // 这里增加了一次fork()

  if(pid == 0){ // 第一次创建子进程

    FILE *fp = fopen("echomsg.txt", "w"); // write txt
    char msg_buf[BUF_SIZE];
    int len;
    for(int i = 0; i < 10;++i){ 
      len = read(fds[0], msg_buf, BUF_SIZE);  // 从管道中读出数据， 这个通道的写入端，在后面的进程中，因此是多进程之间的通信
      fwrite((void*)msg_buf, 1, len, fp); // len 个 1 bytes
    }
    fclose(fp); // 这里要是一直读的话，怎么关闭这个fp呢？？
    return 0;
  }

  while (true)
  {
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size); // 正在阻塞的信号会被挂起，然后去处理信号去
    if (client_sock == -1)
    {
      if (errno == EINTR)
      { // 如果阻塞被信号打断
        continue;
      }
      else
      {
        error_handling("Accept() error.");
      }
    }
    else
      std::cout << "Connected client: " << ++num << std::endl;

    pid = fork(); // 第二次创建子进程

    if (pid == -1) // fork出错
    {
      close(client_sock); // 子进程失败，放弃这个socket
      continue;
    }
    if (pid == 0) // 子进程
    {
      close(server_sock); // 关闭子进程中的 server 的 socket
      while ((str_len = read(client_sock, message, BUF_SIZE)) != 0)
      {
        write(client_sock, message, str_len);
        write(fds[1], message, str_len); // 向管道里写入数据， 因为是在另一个子进程中读的数据，因此相当于是两个进程进行的通信
      }
      close(client_sock);
      puts("client disconnected...");
      return 0;
    }
    else // 父进程
    {
      close(client_sock); // 关闭父类进程中的 client socket， 父进程执行完之后，会开启下一轮的循环
    }
  }

  close(server_sock); // 这里的调用，是父进程和子进程 各执行一遍吗？ 不不不，子进程都在内部 return了

  return EXIT_SUCCESS;
}

/*s intended as a hub of links, documentation and resources related to the project.*/