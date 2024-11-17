#include "todos.h"

/**
 * create_todo - creates new todo structure
 * Return: pointer to todo structure else NULL on failure
 */
todo *create_todo(void)
{
todo *new_todo = (todo *)malloc(sizeof(todo));
if (!new_todo)
return NULL;
new_todo->head = NULL;
new_todo->tail = NULL;
new_todo->nb_tasks = 0;
return (new_todo);
}

/**
 * create_task - create task structure
 * @title: title of task
 * @desc: task description
 * Return: pointer to created task, else NULL
 */
task *create_task(char *title, char *desc)
{
task *new_task;
if (!title || !desc)
return NULL;
new_task = (task *)malloc(sizeof(task));
if (!new_task)
return NULL;
new_task->id = 0;
new_task->title = strdup(title);
new_task->desc = strdup(desc);
new_task->next = NULL;
return (new_task);
}

/**
 * add_task - adds task to todo list
 * @todo_list: pointer to todo struct
 * @new_task: pointer to task to add
 * Return: 1 on success else 0 on failure
 */
int add_task(todo *todo_list, task *new_task)
{
if (!todo_list || !new_task)
return (0);
new_task->id = todo_list->nb_tasks++;
if (!todo_list->head)
{
todo_list->head = new_task;
todo_list->tail = new_task;
}
else
{
todo_list->tail->next = new_task;
todo_list->tail = new_task;
}
return (1);
}

/**
 * free_todo: frees todo structure
 * @todos: pointer to todo structure to free
 * @head: pointer to task to free
 * Return: Nothing
 */
void free_todo(todo *todos, task *head)
{
if (!todos)
return;
if (!head)
{
free(todos);
return;
}
free(head->desc);
free(head->title);
free_todo(todos, head->next);
free(head);
}