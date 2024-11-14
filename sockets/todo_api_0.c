#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 4096

void handle_client(int client_socket);

/**
 * main - opens IPV4/TCP socket and listens on a port
 * Return: 0 always
 */
int main(void)
{
int server_fd, new_socket, valread;
struct sockaddr_in address;
int addrlen = sizeof(address);
server_fd = socket(AF_INET, SOCK_STREAM, 0);
if (server_fd == -1)
{
perror("Socket failed");
exit(EXIT_FAILURE); }
address.sin_family = AF_INET, address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(PORT);
if (bind(server_fd, (struct sockaddr *)&address, addrlen) < 0)
{
perror("Bind failed");
close(server_fd);
exit(EXIT_FAILURE); }
if (listen(server_fd, 3) < 0)
{
perror("Listen failed");
close(server_fd);
exit(EXIT_FAILURE); }
printf("Server listening on port %d\n", PORT);
while (1)
{
printf("Waiting for connection...\n");
new_socket = accept(server_fd, (struct sockaddr *)&address,
(socklen_t *)&addrlen);
if (new_socket < 0)
{
perror("Accept failed");
continue;
}
printf("Client connected: %s\n", inet_ntoa(address.sin_addr));
handle_client(new_socket);
close(new_socket);
printf("Connection closed\n");
}
close(server_fd);
return (0);
}

void handle_client(int client_socket)
{
const char *response;
char buffer[BUFFER_SIZE] = {0};
char method[16], path[256], version[16];
int valread;
valread = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
if (valread <= 0)
{
printf("Failed to read from client.\n");
return;
}
buffer[valread] = '\0';
printf("Received HTTP request:\n%s\n", buffer);
sscanf(buffer, "%15s %255s %15s", method, path, version);
printf("\"\n");
printf("Request breakdown:\n");
printf("Method: %s\n", method);
printf("Path: %s\n", path);
printf("version: %s\n", version);

response =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/plain\r\n"
"Content-Length: 13\r\n"
"\r\n"
"Hellow, world!";
send(client_socket, response, strlen(response), 0);
}