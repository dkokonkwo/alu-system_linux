#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * main - objdump reproduction
 * @argc: num of arguments
 * @argv: array of arguments
 */
int main(int argc, char *argv[])
{
int i, found;
char line[256];
char *command[512];
FILE *fp;
if (argc < 2)
{
fprintf(stderr, "Usage: %s [objfile\n", argv[0]);
return (1);
}
for (i = 1; i < argc; i++)
{
snprintf(command, sizeof(command), "objdump -sf %s", argv[i]);
fp = popen(command, "r");
if (fp == NULL)
{
fprintf(stderr, "Error: failed to excute objdump command.\n");
return (1);
}
found = 0;
while (fgets(line, sizeof(line), fp) != NULL)
{
if (strstr(line, "no symbols") != NULL)
{
fprintf(stderr, "%s: %s: no symbols\n", argv[0], argv[i]);
found = 1;
}
else
{
printf("%s", line);
}
}
pclose(fp);
}
return (found ? 1 : 0);
}
