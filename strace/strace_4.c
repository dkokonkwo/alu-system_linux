#include "syscalls.h"

/**
 * read_string - safely reads a string from the traced process's memory
 * @pid: PID of the traced process
 * @addr: Address in the traced process's memory
 * @buffer: Buffer to store to the read string
 * @max_len: max number of characters to read
 */
void read_string(pid_t pid, unsigned long addr, char *buffer, size_t max_len)
{
    size_t i;
    unsigned long data;

    for (i = 0; i < max_len; i+= sizeof(data))
    {
        data = ptrace(PTRACE_PEEKDATA, pid, addr + i, NULL);
        if (data == -1 && errno != 0)
        {
            perror("ptrace(PTRACE_PEEKDATA)");
            buffer[0] = '\0';
            return;
        }
        memcpy(buffer + i, &data, sizeof(data));
        if (memchr(&data, '\0', sizeof(data)) != NULL)
            break;
    }
    buffer[max_len - 1] = '\0';
}

/**
 * args_print - prints aruguemnt in a syscall table
 * @sys_call: pointer to syscall struct
 * @regs: syscall register
 */
void args_print(const syscall_t *sys_call, struct user_regs_struct *regs, pid_t pid)
{
    size_t i, j;
    unsigned long params[MAX_PARAMS]; /* Maximum number of parameter is 6 */
    params[0] = regs->rdi, params[1] = regs->rsi, params[2] = regs->rdx;
    params[3] = regs->r10, params[4] = regs->r8, params[5] = regs->r9;
    /* reg address are from linux system call table for x86 64 */

    printf("(");
    for (i = 0; sys_call->params[i] != VOID && i < sys_call->nb_params; i++)
    {
        if (i > 0)
            printf(", ");
        if (sys_call->params[i] == VARARGS)
            printf("...");
        else if (sys_call->params[i] == CHAR_P)
        {
            char buffer[256];
            read_string(pid, params[i], buffer, sizeof(buffer));
            printf("\"%s\"", buffer);
        }
        else if (sys_call->params[i] == CHAR_PP)
        {
            printf("[");
            for (j = 0; j < 5; j++)
            {
                unsigned long str_addr;
                str_addr = ptrace(PTRACE_PEEKDATA, pid, params[i] + j * sizeof(unsigned long), NULL);
                if (str_addr == 0)
                    break;
                char buffer[256];
                read_string(pid, str_addr, buffer, sizeof(buffer));
                if (j > 0)
                    printf(", ");
                printf("\"%s\"", buffer);
            }
            printf("]");
        }
        else
            printf("%#lx", params[i]);
    }
    printf(")");
}

/**
 * main - executes and traces a given command
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: 0 sucess, 1 on failure
 */
int main(int argc, char *argv[], char *envp[])
{
int status;
struct user_regs_struct regs;
pid_t child;

if (argc < 2)
{
fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
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
printf("execve(\"%s\", [\"%s\", \"%s\"], [/* 21 vars */]", argv[1], argv[1], argv[2]);
ptrace(PTRACE_TRACEME, child, NULL, NULL);
execve(argv[1], argv + 1, envp);
}
else
{
    while(1)
    {
        if (waitpid(child, &status, 0) == -1)
        {
            perror("waitpid");
            break;
        }
        if (WIFEXITED(status))
            break;
        ptrace(PTRACE_GETREGS, child, NULL, &regs);
        if (regs.orig_rax < SYSCALL_TABLE_SIZE)
        {
            printf("\n%s", syscalls_64_g[regs.orig_rax].name);
            args_print(&syscalls_64_g[regs.orig_rax], &regs, child);
            printf(" = %#lx", (size_t)regs.rax);
        }

        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
    }
}
return (0);
}
