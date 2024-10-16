#include <stdio.h>
#include <stdlib.h>

/**
 * main - replicates the functionality of nm -p
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: 0 on success else 1
 */
int main(int argc, char *argv[])
{
if (argc != 2)
{
fprintf(stderr, "Usage: %s [objfile]\n", argv[0]);
return (1);
}
char command[256];
snprintf(command, sizeof(command), "nm -p %s", argv[1]);
int result = system(command);

if (result != 0)
{
fprintf(stderr, "Error: failed to execute command.\n");
return (1);
}
return (0);
}
