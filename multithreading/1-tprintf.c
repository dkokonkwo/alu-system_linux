#include "multithreading.h"

/**
 * tprintf - usrs printf family to print out string
 * @format: formatted string
 * Return: 0 always
 */
int tprintf(char const *format, ...)
{
pthread_t tid = pthread_self();
print("[%lu] %s", tid, format);
return (0);
}
