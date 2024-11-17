#ifndef TODOS_H
#define TODOS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * task_s - task structure
 * @id: task id
 * @title: task title
 * @desc: task description
 */
typedef struct task_s
{
    int id;
    char *title;
    char *desc;
    struct task_s *next;
} task;

/**
 * todo_s - tdod list structure
 * @nb_tasks: number of tasks
 * @head: pointer to first task
 * @tail: pointer to last task
 */
typedef struct todo_s
{
    int nb_tasks;
    task *head;
    task *tail;
} todo;

todo *create_todo(void);
task *create_task(char *title, char *desc);
int add_task(todo *todo_list, task *new_task);
void free_todo(todo *todos, task *head);

int check_method_and_path(const char *buffer);
int check_header(const char *buffer);
int check_body(const char *body, todo *new_todo);
char *server_response(todo *new_todo);

#endif