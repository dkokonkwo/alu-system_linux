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
 * args_print - prints aruguemnt in a syscall table
 * @sys_call: pointer to syscall struct
 * @regs: syscall register
 */
void args_print(const syscall_t *sys_call, struct user_regs_struct *regs)
{
    size_t i, params[MAX_PARAMS]; /* Maximum number of parameter is 6 */
    params[0] = regs->rdi, params[1] = regs->rsi, params[2] = regs->rdx;
    params[3] = regs->r10, params[4] = regs->r8, params[5] = regs->r9;
    /* reg address are from linux system call table for x86 64 */

    printf("(");
    for (i = 0; sys_call->params[0] != VOID && i < sys_call->nb_params; i++)
    {
        if (sys_call->params[i] == VARARGS)
            printf("%s...", i ? ", " : "");
        else
            printf("%s%#lx", i ? ", " : "", params[i]);
    }
}

/**
 * main - executes and traces a given command
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: 0 sucess, 1 on failure
 */
int main(int argc, char *argv[], char *envp[])
{
int next, status;
struct user_regs_struct regs;
pid_t child;

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
printf("execve(0, 0, 0");
ptrace(PTRACE_TRACEME, child, NULL, NULL);
execve(argv[1], argv + 1, envp);
}
else
{
for (status = 1, next = 0; !WIFEXITED(status); next ^= 1)
{
ptrace(PTRACE_SYSCALL, child, NULL, NULL);
wait(&status);
ptrace(PTRACE_GETREGS, child, NULL, &regs);
if (next)
{
    printf("\n%s", syscalls_64_g[regs.orig_rax].name);
    args_print(&syscalls_64_g[regs.orig_rax], &regs);
}
else if (WIFEXITED(status))
    printf(") = ?\n");
else
    printf(") = %#lx", (size_t)regs.rax);
}
}
return (0);
}
