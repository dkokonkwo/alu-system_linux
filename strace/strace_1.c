#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>
#include "syscalls.h"

/**
 * main - executes and traces a given command
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: 0 sucess, 1 on failure
 */
int main(int argc, char *argv[], char *envp[])
{
int print, status;
struct user_regs_struct regs;
pid_t child;
unsigned long sys_num;

if (argc < 2)
{
fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
return (1);
}

setbuf(stdout, NULL);
child = fork();
if (child == -1)
{
perror("fork");
return (1);
}
else if (child == 0)
{
printf("execve\n");
ptrace(PTRACE_TRACEME, child, NULL, NULL);
execve(argv[1], argv + 1, envp);
}
else
{
for (status = 1, print = 0; !WIFEXITED(status); print ^= 1)
{
ptrace(PTRACE_SYSCALL, child, NULL, NULL);
wait(&status);
ptrace(PTRACE_GETREGS, child, NULL, &regs);
if (print)
{
sys_num = regs.orig_rax;
const char *syscall_name = (sys_num < sizeof(syscalls_64_g)
/ sizeof(syscalls_64_g[0])) ? syscalls_64_g[sys_num].name : "unknown";
printf("%s\n", syscall_name);
}
}
}
return (0);
}
