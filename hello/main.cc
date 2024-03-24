#include <iostream>

int main(){ 

    union{

        short s;
        char c[sizeof(short)];
    } un;

    un.s = 0x0102;

    if(sizeof(short) == 2){
        std::cout <<"yes" << std::endl;
        if(un.c[0] == 2 && un.c[1] == 1) {
            std::cout << "short computert!!\n";
        }
    }
    return 0;
}