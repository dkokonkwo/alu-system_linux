#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

/**
 * main - connects to a listening server
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success, exits with failure otherwise
 */
int main(int argc, char *argv[])
{
int sockfd;
struct sockaddr_in server_addr;
int port;
struct addrinfo hints, *res, *p;
if (argc < 3)
{
printf("Usage: %s <host> <port>\n", argv[0]);
exit(EXIT_FAILURE);
}
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0)
{
perror("Socket creation error");
exit(EXIT_FAILURE);
}
port = atoi(argv[2]);
memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;

if (getaddrinfo(argv[1], argv[2], &hints, &res) != 0)
{
perror("getaddrinfo");
exit(EXIT_FAILURE);
}
for (p = res; p != NULL; p = p->ai_next)
{
sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
if (sockfd < 0)
{
perror("Socket creation error");
continue;
}
if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0)
{
break;
}
perror("Connection failed");
close(sockfd);
}
if (p == NULL)
{
fprintf(stderr, "Failed to connect\n");
freeaddrinfo(res);
exit(EXIT_FAILURE);
}
printf("Connected to %s:%s\n", argv[1], argv[2]);
freeaddrinfo(res);
close(sockfd);
return (0);
}
