#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void timeout(int sig)
{
    if (sig == SIGALRM)
    {
        puts("Time out!");
        alarm(2); //循环设置闹钟
    }
}

void keycontrol(int sig)
{
    if (sig == SIGINT)
        puts("CTRL + C pressed");
}

int main(int argc, char *argv[])
{
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);

    alarm(2);

    for(i = 0; i< 3; ++i){
        puts("wait.....");
        sleep(100); // 阻塞直到有信号到来
    }
    return 0;
}