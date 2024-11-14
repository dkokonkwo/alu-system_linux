#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/**
 * main - connects to a listening server
 * Return: 0 always
 */
int main(int argc, char *argv[])
{
int sockfd;
struct sockadd_in server_addr;
char *host;
int port;

if (argc < 2)
{
printf("Usage: %s <host> <port>\n", argv[0]);
exit(EXIT_FAILURE);
}

if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
{
perror("Socket creation error");
exit(EXIT_FAILURE);
}

port = atoi(argv[1]);
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(port);

if (inet_pton(AF_INET, argv[2], &server_addr.sin_addr) <= 0)
{
perror("Invalid address/ Address not supported");
close(sockfd);
exit(EXIT_FAILURE);
}

if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
{
perror("Connection failed");
close(sockfd);
exit(EXIT_FAILURE);
}
printf("Connected to %s:%s\n", argv[1], argv[2]);

close(sockfd);
return (0);
}
