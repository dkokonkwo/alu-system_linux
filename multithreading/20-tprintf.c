#include "multithreading.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
/**
 * tprintf - usrs printf family to print out string
 * @format: formatted string
 * Return: 0 always
 */
int tprintf(char const *format, ...)
{
va_list args;
pthread_mutex_lock(&mutex);

pthread_t tid = pthread_self();

printf("[%lu]", (unsigned long)tid);

va_start(args, format);
vprintf(format, args);
va_end(args);

pthread_mutex_unlock(&mutex);

return (0);
}
