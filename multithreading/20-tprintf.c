#include "multithreading.h"

/**
 * tprintf - usrs printf family to print out string
 * @format: formatted string
 * Return: 0 always
 */
int tprintf(char const *format, ...)
{
va_list args;
pthread_t tid;
pthread_mutex_lock(&mutex);

tid = pthread_self();

printf("[%lu] ", (unsigned long)tid);

va_start(args, format);
vprintf(format, args);
va_end(args);

pthread_mutex_unlock(&mutex);

return (0);
}

__attribute__((constructor)) void tprintf_mutex_init(void)
{
pthread_mutex_init(&mutex, NULL);
}

__attribute__((destructor)) void tprintf_mutex_destroy(void)
{
pthread_mutex_destroy(&mutex);
}

void end(void) __attribute__((destructor));
