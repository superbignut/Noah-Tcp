#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "net.h"
#include "util.h"
#include "test.h"
#include "driver/fake.h"

static volatile sig_atomic_t terminate;
/* C has the concept of volatile objects. These are normally accessed by pointers and used for accessing hardware or inter-thread communication. 
The standard encourages compilers to refrain from optimizations concerning accesses to volatile objects, but leaves it implementation defined 
as to what constitutes a volatile access. The minimum requirement is that at a sequence point all previous accesses to volatile objects have 
stabilized and no subsequent accesses have occurred. Thus an implementation is free to reorder and combine volatile accesses that occur between sequence points, 
but cannot do so for accesses across a sequence point. The use of volatile does not allow you to violate the restriction on updating objects multiple times between two sequence points.
*/
/* To avoid uncertainty about interrupting access to a variable, you can use a particular data type for which access is always atomic: sig_atomic_t. 
Reading and writing this data type is guaranteed to happen in a single instruction, so there’s no way for a handler to run “in the middle” of an access.*/
static void on_signal(int s){
    (void)s;
    terminate = 1;
}

int main(int argc, char *argv[]){

    struct net_device *dev;

    signal(SIGINT, on_signal);
    if(net_init() == -1){
        _error("net_init() failure");
        return -1;
    }
    dev = fake_init();

    if(!dev){
        _error("fake_init() failure");
        return -1;
    }

    if(net_run() == -1){
        _error("net_run failure");
        return -1;
    }
    while(!terminate){
        if(net_device_output(dev, 0x8000, test_data, sizeof(test_data), NULL) == -1){
            _error("net_device_output() failure");
            break;
        }
        sleep(1);
    }
    net_shutdown();
    return 0;
}

