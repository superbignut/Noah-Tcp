#ifndef __UTIL_H_
#define __UTIL_H_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
/*
    Compare
*/

#ifndef MAX
    #define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
    #define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

/*  
    Array
*/

/* Like size() function */
#define count_of(x) (sizeof(x) / sizeof(*x))
/* Like end() function */
#define tail_of(x) (x + count_of(x))
/* uintptr_t is unsigned long */
#define index_of(x, y) (((uintptr_t)y - (uintptr_t)x) / sizeof(*y))

/*
    Time
*/

#define timeval_add_usec(x, y) 

#define timespec_add_nsec(x, y) 

/*
    Logging
*/


#define _error(...) log_printf(stderr, 'E', __FILE__, __LINE__, __func__, __VA_ARGS__)
#define  _warn(...) log_printf(stderr, 'W', __FILE__, __LINE__, __func__, __VA_ARGS__)
#define  _info(...) log_printf(stderr, 'I', __FILE__, __LINE__, __func__, __VA_ARGS__)
#define _debug(...) log_printf(stderr, 'D', __FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef HEXDUMP
    #define debugdump(...) hexdump(stderr, _VA_ARGS__)
#else
    #define debugdump(...)
#endif

int log_printf(FILE *fp, int level, const char *file, int line, const char *func, const char *fmt, ...);

void hex_dump(FILE *fp, const void *data, size_t size);



/*
    Queue
*/

struct queue_entry;

struct queue_head {
    struct queue_entry *head;
    struct queue_entry *tail;
    unsigned int num;
};


void help();

#endif