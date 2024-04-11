#include <iostream>
#include <string.h>
int main()
{
    int a;
    char msg[30];

    fgets(msg,  30, stdin);

    std::cout << strlen(msg)<< std::endl;

    std::cout << msg << std::endl;
    return 0;
}