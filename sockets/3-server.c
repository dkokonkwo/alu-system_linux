#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

/**
 * main - opens IPV4/TCP socket and listens on a port
 * Return: 0 always
 */
int main(void)
{
int server_fd, new_socket, valread;
struct sockaddr_in address;
int addrlen = sizeof(address);
char buffer[BUFFER_SIZE] = {0};
server_fd = socket(AF_INET, SOCK_STREAM, 0);
if (server_fd == -1)
{
perror("Socket failed");
exit(EXIT_FAILURE);
}
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(PORT);
if (bind(server_fd, (struct sockaddr *)&address, addrlen) < 0)
{
perror("Bind failed");
close(server_fd);
exit(EXIT_FAILURE);
}
if (listen(server_fd, 3) < 0)
{
perror("Listen failed");
close(server_fd);
exit(EXIT_FAILURE);
}
printf("Server listening on port %d\n", PORT);
new_socket = accept(server_fd, (struct sockaddr *)&address,
(socklen_t *)&addrlen);
if (new_socket < 0)
{
perror("Accept failed");
close(server_fd);
exit(EXIT_FAILURE);
}
printf("Client connected: %s\n", inet_ntoa(address.sin_addr));
while (1)
{
memset(buffer, 0, BUFFER_SIZE);
valread = recv(new_socket, buffer, BUFFER_SIZE, 0);
if (valread <= 0)
{
printf("Client disconnected.\n");
break;
}
printf("Message received: %s\n", buffer);
break;
}
close(new_socket);
close(server_fd);
return (0);
}
