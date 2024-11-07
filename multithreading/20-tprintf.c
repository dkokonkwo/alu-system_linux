#include "multithreading.h"

/**
 * tprintf - usrs printf family to print out string
 * @format: formatted string
 * Return: 0 always
 */
int tprintf(char const *format, ...)
{
pthread_mutex_lock(&mutex);
pthread_t tid = pthread_self();
printf("[%lu] %s", tid, format);
pthread_pthread_mutex_unlock(&mutex);
return (0);
}
