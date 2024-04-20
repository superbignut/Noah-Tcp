#include <stdio.h>

#include "util.h"
#include "test.h"
#include "platform.h"
#include <pthread.h>

mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

int count = 10;

void *print_msg(void *ptr){
    char *msg ;
    msg = (char*)ptr;

    pthread_mutex_lock(&m1);
    count++;
    printf("%s %d\n", msg, count);
    pthread_mutex_unlock(&m1);

}



int main(){

    _error("%s, %d, %d", "this is an error!", 1234, 12345);

    debug_dump(test_data, sizeof(test_data) -10);

    pthread_t t1, t2;

    char *msg1 = "Thread 1";
    char *msg2 = "Thread 2";

    int r1, r2;

    r1 = pthread_create(&t1, NULL, print_msg, msg1);
    r2 = pthread_create(&t2, NULL, print_msg, msg2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

/*     printf("Thread 1 returns: %d\n",r1);
    printf("Thread 2 returns: %d\n",r2); */

    return 0;
}