#include <stdio.h>

#include "util.h"
#include "test.h"

int main(){

    _error("%s, %d, %d", "this is an error!", 1234, 12345);

    debug_dump(test_data, sizeof(test_data) -10);


    uint16_t aaa = 0b00000001;
    //aaa = hton16(aaa);
    printf("%04b\n", ~aaa);

    return 0;
}