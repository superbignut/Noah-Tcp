#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, char *argv[])
{

    int status;

    pid_t pid = fork();

    if (pid == 0) // son
    {
        return 3;
    }
    else // father
    {   
        printf("Child pid: %d\n", pid);
        pid = fork();
        if(pid == 0){
            exit(7);
        }
        else{
            printf("Chile PID %d \n", pid);
            wait(&status);
            if(WIFEXITED(status))
                printf("Child send one %d \n", WEXITSTATUS(status));

            wait(&status);
            if(WIFEXITED(status))
                printf("Child send two %d \n", WEXITSTATUS(status));
            sleep(30);
        }
    }
    return 0;
}