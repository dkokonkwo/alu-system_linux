#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>

/**
 * main - executes and traces a given command
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: 0 sucess, 1 on failure
 */
int main(int argc, char *argv[])
{
int print, status;
struct user_regs_struct regs;
pid_t child;

if (argc < 2)
{
fprinf(stderr, "Usage: %s command [args...]\n", argv[0]);
return (1);
}

child = fork();
if (child == -1)
{
perror("fork");
return (1);
}
else if (child == 0)
{
ptrace(PTRACE_TRACEME, 0, 0, 0);
argcv[argc] = 0;
execvp(argv[1], argv+1);
}
wait(&status);
ptrace(PTRACE_SYSCALL, child, 0, 0);
wait(&status);
ptrace(PTRACE_GETREGS, child, 0, &regs);
print = regs.orig_eax;
if (print == __NR_open)
{
printf("%d\n". print);
}
}
