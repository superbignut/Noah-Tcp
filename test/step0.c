#include <stdio.h>
#include "util.h"
int main(){
    int a  =10;
    int b = 12;
    int c = MAX(a, b);

    _error("%s, %d, %d", "this is an error!", 1234, 12345);

    return 0;
}