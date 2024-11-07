#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/**
 * get_prime - finds next prime number
 * @initial: intial value
 * Returns: next prime number
 */
unsigned long get_prime(unsigned long initial)
{
unsigned long i, num;
num = initial + 1;

while (1)
{
int is_prime = 1;

for (i = 2; i <= num / 2; i++)
{
if (num % i == 0)
{
is_prime = 0;
break;
}
}

if (is_prime)
return (num);

num++;
}
}


/**
 * prime_factors - factorizes a number into a list of prime factors
 * @s: string number
 * Return: pointer to list of prime factors
 */
list_t *prime_factors(char const *s)
{
unsigned long num, divisor, *factor;
char *end;
list_t *list;
if (!s || strlen(s) == 0)
return (NULL);

num = strtoul(s, &end, 10);
if (*end != '\0')
return (NULL);

list = malloc(sizeof(list_t));
if (!list)
return (NULL);
if (!list_init(list))
{
free(list);
return (NULL);
}

divisor = 1;
while (num > 1)
{
divisor = get_prime(divisor);

while (num % divisor == 0)
{
factor = malloc(sizeof(unsigned long));
if (!factor)
{
list_destroy(list, free);
free(list);
return (NULL);
}

*factor = divisor;
if (!list_add(list, factor))
{
free(factor);
list_destroy(list, free);
free(list);
return (NULL);
}

num /= divisor;
}
}

return (list);
}