#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "net.h"
#include "util.h"
#include "test.h"
#include "driver/fake.h"

static volatile sig_atomic_t terminate;

static void on_signal(int s){
    (void)s;
    terminate = 1;
}

int main(int argc, char *argv[]){

    return 0;
}

