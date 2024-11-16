#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 4096

void handle_client(int client_socket);
void parse_headers(const char *buffer);

/**
 * main - opens IPV4/TCP socket and listens on a port
 * Return: 0 always
 */
int main(void)
{
int server_fd, new_socket;
struct sockaddr_in address;
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
while (1)
{
new_socket = accept(server_fd, (struct sockaddr *)&address,
(socklen_t *)&addrlen);
if (new_socket < 0)
{
perror("Accept failed");
continue;
}
printf("Client connected: %s\n", inet_ntoa(address.sin_addr));
fflush(stdout);
handle_client(new_socket);
close(new_socket);
}
close(server_fd);
return (0);
}

void handle_client(int client_socket)
{
const char *response;
char buffer[BUFFER_SIZE] = {0};
int valread;
valread = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
if (valread <= 0)
{
printf("Failed to read from client.\n");
return;
}
buffer[valread] = '\0';
printf("Raw request: \"%s", buffer);
fflush(stdout);
printf("\"\n");
parse_headers(buffer);
fflush(stdout);
response =
"HTTP/1.1 200 OK\r\n"
"\r\n";
send(client_socket, response, strlen(response), 0);
}

void parse_headers(const char *buffer)
{
const char *header_start, *header_end;
char key[50], value[50];
header_start = strstr(buffer, "\r\n") + 2;
if (!header_start)
return;
while (header_start && *header_start != '\r' && *header_start != '\n')
{
header_end = strstr(header_start, "\r\n");
if (!header_end)
break;
sscanf(header_start, "%[^:]: %[^\r\n]", key, value);
printf("Header: \"%s\" -> \"%s\"\n", key, value);
header_start = header_end + 2;
}
}
