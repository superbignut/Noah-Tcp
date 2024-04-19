#include <stdio.h>
#include <stdarg.h>
//#include <stdint.h>
#include <time.h> // Standard
#include <sys/time.h> // POSIX
#include <ctype.h> // isascii
//#include <stdlib.h>
//#include <string.h>
#include "platform.h"
#include "util.h"


/*
    !Logging
*/


/// @brief 打印函数,被ERROR, WARNING, INFO, DEBUG 宏展开后间接调用,以 _error(...)举例：
///        _error("%s, %d", "Hello", 123); 其中格式化字符串传给fmt, Hello和123传给可变参数
///        在log_printf 函数内部,使用vfprintf 将可变参数以fmt的格式输出,并附加时间和位置信息
/// @param fp 
/// @param level 
/// @param file 
/// @param line 
/// @param func 
/// @param fmt 
/// @param 
/// @return 打印的总字节数
int log_printf(FILE *fp, int level, const char *file, int line, const char *func, const char *fmt, ...)
{
    struct timeval tv_time;
    struct tm tm_time;
    char time_stamp[32];
    int num_to_printf = 0;
    va_list ap;

    /* The flockfile() function waits for *filehandle to be no longer locked by a different thread, then makes the current thread owner
       of *filehandle, and increments the lockcount. */
    flockfile(fp);

    /* Gives the number of seconds and microseconds since the Epoch */
    gettimeofday(&tv_time, NULL);

    /* localtime_r Converts given time into calendar time, expressed in local time, in the struct tm format. The function uses user-provided storage tm for the result. */
    /* Converts the date and time information from a given calendar time tp to a null-terminated multibyte character string str according to format string format */
    /* %T means: %H:%M:%S */
    strftime(time_stamp, sizeof(time_stamp), "%T", localtime_r(&tv_time.tv_sec, &tm_time));

    /* Use cpp -dM to show all predefined macro. */

    num_to_printf += fprintf(fp, "%s.%03d [%c] %s: ", time_stamp, (int)(tv_time.tv_usec / 1000), level, func);

    /* The va_start macro enables access to the variable arguments following the named argument parm_n */
    /* receive the variable after fmt */
    va_start(ap, fmt);

    /* print va_list by the format fmt */
    num_to_printf += vfprintf(fp, fmt, ap);
    /* Clean the va_list ap */
    va_end(ap);

    num_to_printf += fprintf(fp, " (%s:%d)\n", file, line);

    /* The funlockfile() function decrements the lock count. */
    funlockfile(fp);

    return num_to_printf;
}

/// @brief 打印字符串数组的每个字节，核心使用fprintf(fp, "%02x ", src);
/// @param fp 
/// @param data 
/// @param size 
void hex_dump(FILE *fp, const void *data, size_t size){
    unsigned char *src;
    int offset, index;

    flockfile(fp);

    src = (unsigned char *)data;
    fprintf(fp, "+------+-------------------------------------------------+------------------+\n");
    for(offset = 0; offset < (int)size; offset+=16){
        fprintf(fp, "| %04x | ", offset);
        for(index = 0; index < 16; index++){
            if(offset + index < (int) size)
                fprintf(fp, "%02x ", src[offset + index]);
            else
                fprintf(fp, "   "); // 3 spaces.
        }
        fprintf(fp, "| ");

        for(index = 0; index < 16; index++){
            if(offset + index < (int) size){
                if(isascii(src[index + offset]) && isprint(src[offset + index])){
                    fprintf(fp, "%c", src[offset + index]);
                }
                else {
                    fprintf(fp, ".");
                }
            }
            else{
                fprintf(fp, " ");
            }
        }
        fprintf(fp, " |\n");
    }
    fprintf(fp, "+------+-------------------------------------------------+------------------+\n");
    funlockfile(fp);
}

void help(){

}

/*
    !Queue
*/
/*
struct queue_entry{
    struct queue_entry *next;
    void *data;
};

struct queue_head {
    struct queue_entry *head;
    struct queue_entry *tail;
    unsigned int num;
};
*/

/// @brief Init the queue, both head_ptr and tail_ptr is NULL.
/// @param queue 
void queue_init(struct queue_head *queue){
    queue->head = NULL;
    queue->tail = NULL;
    queue->num = 0;
}

/// @brief Push option of the queue (data is added after tail).
/// @param queue 
/// @param data 
/// @return data when success, NULL when failure.
void *queue_push(struct queue_head *queue, void *data){
    if(queue == NULL) /* queue is not init !*/
        return NULL;

    struct queue_entry* entry;
    entry = memory_alloc(sizeof(*entry));

    if(entry == NULL) /* alloc error! */
        return NULL;
    
    entry->next = NULL;
    entry->data = data;

    if(queue->tail != NULL)
        queue->tail->next = entry;
    
    queue->tail = entry;

    if(queue->head == NULL)
        queue->head = entry;
    queue->num ++;  

    return data;
}

/// @brief Pop option of the queue (update the queue->head).
/// @param queue 
/// @return The data in original queue->head
void *queue_pop(struct queue_head *queue){
    if(queue == NULL)
        return NULL;
    if(queue->head == NULL)
        return NULL;

    struct queue_entry* entry;
    void *data;

    entry = queue->head;
    data = entry->data;

    if(queue->head == queue->tail) /* just one node */
        queue->tail = NULL;

    queue->head = entry->next;
    queue->num--;

    memory_free(entry);

    return data;
}

/// @brief Check queue->head->data.
/// @param queue 
/// @return queue->head->data when queue is not empty, else NULL.
void *queue_peek(struct queue_head *queue){
    if(queue == NULL || queue->head == NULL)
        return NULL;
    return queue->head->data;
}

/// @brief Iterate over the entire queue with the func and arg.
/// @param queue 
/// @param func 
/// @param arg 
void queue_foreach(struct queue_head *queue, void(*func)(void *arg, void *data), void *arg){

    if(queue == NULL || queue->head == NULL)
        return;
    struct queue_entry* entry;

    for(entry = queue->head; entry != NULL; entry = entry->next)
        func(arg, entry->data);
    return;
}


/*
    !ByteOrder
*/

#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN 4321
#endif

#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN 1234
#endif

static int endian = 0; 

/// @brief Return the endian of this computer.
/// @return 
static int byteorder(){
    uint32_t x = 0x00000001;
    return *(uint8_t*)x ? __LITTLE_ENDIAN : __BIG_ENDIAN;
}

/// @brief 0x01_23 => 0x23_01
/// @param v 
/// @return 
static uint16_t byteswap16(uint16_t v){
    return ((v & 0x00ff) << 8 )|( (v & 0xff00) >> 8);
}

/// @brief 0x01_23_45_67 => 0x67_45_23_01
/// @param v 
/// @return 
static uint32_t byteswap32(uint32_t v){
    return ((v & 0x000000ff) << 24 )|( (v & 0xff000000) >> 24) | ((v & 0x00ff0000) >> 8) | ((v & 0x0000ff00) << 8);
}


uint16_t hton16(uint16_t h){
    if(endian == 0){
        endian = byteorder;
    }
    return endian == __BIG_ENDIAN ? h : byteswap16(h);
}

uint16_t ntoh16(uint16_t n){
    if(endian == 0){
        endian = byteorder;
    }
    return endian == __BIG_ENDIAN ? n : byteswap16(n);
}

uint32_t hton32(uint32_t h){
    if(endian == 0){
        endian = byteorder;
    }
    return endian == __BIG_ENDIAN ? h : byteswap32(h);
}

uint32_t ntoh32(uint32_t n){
    if(endian == 0){
        endian = byteorder;
    }
    return endian == __BIG_ENDIAN ? n : byteswap32(n);
}

/// @brief Compute the checksum of the given array and init.
/// @param addr 
/// @param count 
/// @param init 
/// @return One's complement of 16 bit sum.
uint16_t check_sum16(uint16_t *addr, uint16_t count, uint32_t init){

    uint32_t sum;

    sum = init;

    while(count > 1){
        sum += *addr;
        addr++;
        count -= 2;
    }
    if(count == 1)
        sum += *(uint8_t*)addr;

    while(sum >> 16 != 0)
        sum = (sum & 0x0000ffff) + (sum >> 16);
    return ~(uint16_t)sum;
}

