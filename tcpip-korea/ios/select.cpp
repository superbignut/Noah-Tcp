    #include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{

    fd_set reads, temps;

    int result, str_len;

    char buf[BUF_SIZE];

    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads); // 将第0位设置为1，监视标准输入的变化

/*     timeout.tv_sec = 5;
    timeout.tv_usec = 5000; */

    while (true)
    {
        temps = reads; // 复制
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000; //select结束后，这两个会变成超时前剩余时间

        result = select(1, &temps, 0, 0, &timeout); 
        if (result == -1)
        {
            puts("select() error!");
            break;
        }
        else if (result == 0)
        {
            puts("Time-out!");
        }
        else
        {
            if (FD_ISSET(0, &temps))
            {
                str_len = read(0, buf, BUF_SIZE); // 0代表标准输入，1代表标准输出，2代表标准错误
                
                buf[str_len] = 0;
                //write(1,buf,str_len+1);`1
                printf("message from console: %s", buf);
            }
        }
    }

    return 0;
}

// select 用来建监视三种监视项的变化情况，接收，传输，异常
// 根据变化情况，向三个fd_set注册文件描述符信息