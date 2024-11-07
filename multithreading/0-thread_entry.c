#include "multithreading.h"

/**
 * thread_entry - entry point to a new thread
 * @arg: address of string to be printed
 */
void *thread_entry(void *arg)
{
    printf("%s\n", (char *)arg);
    pthread_exit(NULL);
}
