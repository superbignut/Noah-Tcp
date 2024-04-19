#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#include <stddef.h> // size_t 
#include <stdlib.h> // calloc 
#include <pthread.h>

/*
    !Memory
*/

/* 如果不加static的话， memory_alloc会被认为是外部函数，但在链接时又找不到定义，所以会报错undefined reference */
/* 加了static之后， #include 这个头文件的.c文件中会认为 memory_alloc 是内部函数，不需要去其他文件寻找定义 */
/* 可以使用 nm objdumt -t 指令进行查看 */

/// @brief 分配size大小的动态内存并初始化为0
/// @param size 
/// @return 
static inline void *memory_alloc(size_t size){
    /* calloc() gives you a zero-initialized buffer, while malloc() leaves the memory uninitialized. */
    /* Some compilers even can optimize malloc + memset(0) into calloc for you, but it's best to just
    use calloc in the source if you want zeroed memory. */
    return calloc(1, size);
    /* On success, calloc() returns the pointer to the beginning of newly allocated memory.
     To avoid a memory leak, the returned pointer must be deallocated with free() or realloc().*/
    /* On failure, returns a null pointer. */
}

/// @brief 
/// @param ptr 
static inline void memory_free(void *ptr){
    /* Deallocates the space previously allocated by malloc(), calloc(), aligned_alloc(),(since C11) or realloc().*/
    /* If ptr is a null pointer, the function does nothing. */
    /* The behavior is undefined if the value of ptr does not equal a value returned earlier by malloc(), calloc(), realloc(), or aligned_alloc()(since C11).*/
    /* The behavior is undefined if the memory area referred to by ptr has already been deallocated */
    /* The behavior is undefined if after free() returns, an access is made through the pointer ptr 
        (unless another allocation function happened to result in a pointer value equal to ptr).*/
    /* free is thread-safe: it behaves as though only accessing the memory locations visible through its argument, and not any static storage.*/
    free(ptr);
}

#endif