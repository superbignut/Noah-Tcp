#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#include <stddef.h> // size_t
#include <stdlib.h> // calloc
#include <pthread.h>/* The POSIX thread libraries are a standards based thread API for C/C++. 
It allows one to spawn a new concurrent process flow. It is most effective on multi-processor or multi-core systems where the process flow can be 
scheduled to run on another processor thus gaining speed through parallel or distributed processing. Threads require less overhead than "forking" or 
spawning a new process because the system does not initialize a new system virtual memory space and environment for the process. While most effective 
on a multiprocessor system, gains are also found on uniprocessor systems which exploit latency in I/O and other system functions which may halt process 
execution.(One thread may execute while another is waiting for I/O or some other system latency.)*/

/*
POSIX.1 specifies a set of interfaces (functions, header files)
       for threaded programming commonly known as POSIX threads, or
       Pthreads.  A single process can contain multiple threads, all of
       which are executing the same program.  These threads share the
       same global memory (data and heap segments), but each thread has
       its own stack (automatic variables).
*/

/*
Threads in the same process share:
    Process instructions
    Most data
    open files (descriptors)
    signals and signal handlers
    current working directory
    User and group id
Each thread has a unique:
    Thread ID
    set of registers, stack pointer
    stack for local variables, return addresses
    signal mask
    priority
    Return value: errno
*/


/*
    !Memory
*/

/*  nm and objdumt -t  */

/// @brief Allocate size bytes memory and init with 0.
/// @param size
/// @return
static inline void *memory_alloc(size_t size)
{
    /* calloc() gives you a zero-initialized buffer, while malloc() leaves the memory uninitialized. */
    /* Some compilers even can optimize malloc + memset(0) into calloc for you, but it's best to just
    use calloc in the source if you want zeroed memory. */
    return calloc(1, size);
    /* On success, calloc() returns the pointer to the beginning of newly allocated memory.
     To avoid a memory leak, the returned pointer must be deallocated with free() or realloc().*/
    /* On failure, returns a null pointer. */
}

/// @brief Just free.
/// @param ptr
static inline void memory_free(void *ptr)
{
    /* Deallocates the space previously allocated by malloc(), calloc(), aligned_alloc(),(since C11) or realloc().*/
    /* If ptr is a null pointer, the function does nothing. */
    /* The behavior is undefined if the value of ptr does not equal a value returned earlier by malloc(), calloc(), realloc(), or aligned_alloc()(since C11).*/
    /* The behavior is undefined if the memory area referred to by ptr has already been deallocated */
    /* The behavior is undefined if after free() returns, an access is made through the pointer ptr
        (unless another allocation function happened to result in a pointer value equal to ptr).*/
    /* free is thread-safe: it behaves as though only accessing the memory locations visible through its argument, and not any static storage.*/
    free(ptr);
}

/*
    !Mutex
*/

/*
Mutexes are used to prevent data inconsistencies due to race conditions. A race condition often occurs when two or more threads
need to perform operations on the same memory area, but the results of computations depends on the order in which these 
operations are performed. Mutexes are used for serializing shared resources. Anytime a global resource is accessed by more than 
one thread the resource should have a Mutex associated with it. One can apply a mutex to protect a segment of memory 
("critical region") from other threads. Mutexes can be applied only to threads in a single process and do not work between processes as do semaphores.
*/

/*
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int count = 10;

void functionC()
{
   pthread_mutex_lock( &mutex1 );
   counter++
   pthread_mutex_unlock( &mutex1 );
}
*/

typedef pthread_mutex_t mutex_t;

#define MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER

static inline int mutex_init(mutex_t *mutex)
{
    return pthread_mutex_init(mutex, NULL);
}

static inline int mutex_lock(mutex_t *mutex)
{
    return pthread_mutex_lock(mutex);
}

static inline int mutex_unlock(mutex_t *mutex)
{
    return pthread_mutex_unlock(mutex);
}

#endif