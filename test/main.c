#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>

#include "net.h"
#include "util.h"
#include "driver/fake.h"
#include "test.h"

static volatile sig_atomic_t terminate = 0;

void on_signal(int s){
    (void)s;
    terminate = 1;
}

int main(){

    struct net_device *dev;

    signal(SIGINT,on_signal);

    if(net_init() == -1){
        printf("net init failure\n");
    }

    dev = fake_init();

    if(dev == NULL){
        printf("fake init failure\n");
        return -1;
    }

    if(net_run() == -1){
        _error("net_run failure");
        return -1;
    }

    while(!terminate){
        if(net_device_output(dev, 0x8000, test_data, sizeof(test_data),NULL) == -1){
            _error("net_device_output() failure");
            break;
        }
        sleep(1);
    }
    net_shutdown();
    return 0;
}