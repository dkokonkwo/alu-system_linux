#include "todos.h"
/**
 * check_method_and_path - checks whether method and path are correct
 * @buffer: request
 * Return: 1 on success, else 0 on failutre
 */
int check_method_and_path(const char *buffer)
{
char method[16], path[256], version[16];
if (sscanf(buffer, "%15s %255s %15s", method, path, version) != 3)
return (0);
if (strcmp(method, "POST") != 0 || strcmp(path, "/todos") != 0)
return (0);
return (1);
}

/**
 * check_header - checks header for Content-Length
 * @buffer: pointer to raw request
 * Return: 1 if Content-Length, else 0 if not found
 */
int check_header(const char *buffer)
{
const char *header_start, *header_end;
char key[50], value[50];
header_start = strstr(buffer, "\r\n");
if (!header_start)
return (0);
header_start += 2;
while (header_start && *header_start != '\r' && *header_start != '\n')
{
header_end = strstr(header_start, "\r\n");
if (!header_end)
break;
if (sscanf(header_start, "%49[^:]: %49[^\r\n]", key, value) == 2)
{
if (strcmp(key, "Content-Length") == 0)
return 1;
}
header_start = header_end + 2;
}
return (0);
}

/**
 * check_body - checks title and description are present
 * @body: pointer to body
 * @new_todo: pointer to todo list
 * Return: 1 if new task created else 0 on failure
 */
int check_body(const char *body, todo *new_todo)
{
task *new_task;
char *body_copy, *token;
char key[256], value[256], title[256] = "", desc[256] = "";
body_copy = strdup(body);
if (!body_copy)
return (0);
token = strtok(body_copy, "&");
while (token)
{
if (sscanf(token, "%255[^=]=%255s", key, value) == 2)
{
if (strcmp(key, "title") == 0)
strncpy(title, value, sizeof(title) - 1);
else if (strcmp(key, "description") == 0)
strncpy(desc, value, sizeof(desc) - 1);
}
token = strtok(NULL, "&");
}
free(body_copy);
if (title[0] == '\0' || desc[0] == '\0')
return (0);
new_task = create_task(title, desc);
if (!new_task)
return (0);
if (!add_task(new_todo, new_task))
{
free(new_task->title);
free(new_task->desc);
free(new_task);
return (0);
}
return (1);
}

char *server_response(todo *new_todo)
{
char *response, *json_body;
task *last_task;
size_t response_size, body_size;
if (!new_todo || !new_todo->tail)
return NULL;

last_task = new_todo->tail;
body_size = snprintf(NULL, 0,
"{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
last_task->id, last_task->title, last_task->desc);
json_body = malloc(body_size + 1);
if (!json_body)
return NULL;
snprintf(json_body, body_size + 1,
"{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
last_task->id, last_task->title, last_task->desc);

response_size = snprintf(NULL, 0,
"HTTP/1.1 201 Created\r\n"
"Content-Length: %lu\r\n"
"Content-Type: application/json\r\n"
"\r\n"
"%s",
(unsigned long)body_size, json_body);
response = malloc(response_size + 1);
if (!response)
{
free(json_body);
return NULL;
}
snprintf(response, response_size + 1,
"HTTP/1.1 201 Created\r\n"
"Content-Length: %lu\r\n"
"Content-Type: application/json\r\n"
"\r\n"
"%s",
(unsigned long)body_size, json_body);

free(json_body);
return response;
}