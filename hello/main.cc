#include <iostream>
#include <string.h>
int main()
{

    struct test
    {
        int b = 2;
        long a = 1;
    };

    struct test aaa;
    memset(&aaa,0, sizeof(test));

    std::cout << aaa.b << " " << aaa.b << " " << sizeof(test);
    return 0;
}