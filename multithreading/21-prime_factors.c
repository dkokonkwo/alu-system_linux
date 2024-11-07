#include "list.h"

/**
 * get_prime - finds next prime number
 * @initial: intial value
 * Returns: next prime number
 */
unsigned long get_prime(unsigned long initial)
{
unsigned long final, i;
final = initial + 1;
if (final == 2 || final == 3)
return (final);
while (1)
{
for (i = 2; final % i != 0; i++)
{
if (i > (final / 2))
return (final);
}
final++;
}
}


/**
 * prime_factors - factorizes a number into a list of prime factors
 * @s: string number
 * Return: pointer to list of prime factors
 */
list_t *prime_factors(char const *s)
{
list_t *list;
node_t *node;
unsigned long num, i, pprime, divisor = 1;
int n, j;
if (!s)
    return (NULL);

list = (list_t *)malloc(sizeof(list_t));
if (!list)
{
printf("Failed to create list\n");
return (NULL);
}
list = list_init(list);
num = 0;
i = 1;
for (n = strlen(s) - 1; n != -1; n--)
{
num += (s[n] - '0' - '\0') * i;
i *= 10;
}

while (num > 1)
{
divisor = get_prime(divisor);
if (divisor > (num / 2))
{
node = list_add(list, num);
if (!node)
{
list_destroy(list, NULL);
return (NULL);
}
return (list);
}
if (num % divisor == 0)
{
node = list_add(list, divisor);
if (!node)
{
list_destroy(list, NULL);
return (NULL);
}
num = num / divisor;
divisor = 1;
}
}
return (list);
}