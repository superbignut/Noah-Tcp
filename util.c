#include <stdio.h>
#include <stdarg.h>
//#include <stdint.h>
#include <time.h> // Standard
#include <sys/time.h> // POSIX

#include "util.h"

/// @brief 打印函数,被ERROR, WARNING, INFO, DEBUG 宏展开后间接调用,以 _error(...)举例
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

void help(){

    // printf("Compile Successfully!\n");
}