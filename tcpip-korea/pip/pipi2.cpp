#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]){

    int fds[2];
    char str1[] = "who are you?";

    char str2[] = "Thank you!";

    char buf[BUF_SIZE];

    pid_t pid;

    pipe(fds);

    pid = fork();

    if(pid == 0){
        write(fds[1], str1, sizeof(str1));
        sleep(2);
        read(fds[0], buf, BUF_SIZE);

        printf("1%s\n", buf);
    }else{
        read(fds[0], buf, BUF_SIZE);
        printf("2%s\n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3); // 如果父进程提前结束，子进程会变成孤儿进程，进而被linux继续执行
                    // 所以这里的sleep(3) 的目的是等待子进程执行完毕，变成僵尸进程
    }

    return 0;
}