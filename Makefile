CC = gcc
CFLAGS = -Wall -g

strace_0: strace_0.c
	$(CC) $(CFLAGS) -o strace_0 strace_0.c

clean:
	rm -f strace_0
