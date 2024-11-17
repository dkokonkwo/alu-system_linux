#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "todos.h"

#define PORT 8080
#define BUFFER_SIZE 4096

void handle_client(int client_socket, char *ip_addr, todo *new_todo);

/**
 * main - opens IPV4/TCP socket and listens on a port
 * Return: 0 always
 */
int main(void)
{
int server_fd, new_socket;
struct sockaddr_in address;
todo *todos;
int addrlen = sizeof(address);
server_fd = socket(AF_INET, SOCK_STREAM, 0);
if (server_fd == -1)
{
perror("Socket failed");
exit(EXIT_FAILURE); }
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
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
fflush(stdout);
todos = create_todo();
if (!todos)
{
perror("Failed to create todo list");
exit(EXIT_FAILURE);
}
while (1)
{
new_socket = accept(server_fd, (struct sockaddr *)&address,
(socklen_t *)&addrlen);
if (new_socket < 0)
{
perror("Accept failed");
continue;
}
handle_client(new_socket, inet_ntoa(address.sin_addr), todos);
close(new_socket);
}
close(server_fd);
free_todo(todos, todos->head);
return (0);
}

void handle_client(int client_socket, char *ip_addr, todo *new_todo)
{
const char *response, *body_start;
char method[16], path[256], version[16];
char buffer[BUFFER_SIZE] = {0};
int valread, success;
valread = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
if (valread <= 0)
{
printf("Failed to read from client.\n");
return;
}
buffer[valread] = '\0';
sscanf(buffer, "%15s %255s %15s", method, path, version);
success = check_method_and_path(buffer);
if (!success)
{
response = "HTTP/1.1 404 Not Found\r\n\r\n";
send(client_socket, response, strlen(response), 0);
printf("%s %s %s -> 404 Not Found\n", ip_addr, method, path);
fflush(stdout);
return;
}
success = check_header(buffer);
if (!success)
{
response = "HTTP/1.1 411 Length Required\r\n\r\n";
send(client_socket, response, strlen(response), 0);
printf("%s %s %s -> 411 Length Required\n", ip_addr, method, path);
fflush(stdout);
return;
}
body_start = strstr(buffer, "\r\n\r\n");
if (body_start)
{
body_start += 4;
success = check_body(body_start, new_todo);
if (!success)
{
response = "HTTP/1.1 422 Unprocessable Entity\r\n\r\n";
send(client_socket, response, strlen(response), 0);
printf("%s %s %s -> 422 Unprocessable Entity\n", ip_addr, method, path);
fflush(stdout);
return;
}
}
response = server_response(new_todo);
if (response)
{
send(client_socket, response, strlen(response), 0);
printf("%s %s %s -> 201 Created\n", ip_addr, method, path);
}
else
{
response = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
send(client_socket, response, strlen(response), 0);
printf("%s %s %s -> 500 Internal Server Error\n", ip_addr, method, path);
}
}

