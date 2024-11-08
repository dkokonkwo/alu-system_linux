#include "multithreading.h"


__attribute__((constructor)) void task_mutex_init(void)
{
pthread_mutex_init(&task_mutex, NULL);
}

__attribute__((destructor)) void task_mutex_destroy(void)
{
pthread_mutex_destroy(&task_mutex);
}

/**
 * create_task - creates a task structure
 * @entry: pointer to the entry function of the task
 * @param: parameter that will be passed to the entry function
 * Return: pointer to created task
 */
task_t *create_task(task_entry_t entry, void *param)
{
task_t *task;
static unsigned int id;
task = malloc(sizeof(task_t));
if (!task)
return (NULL);
task->entry = entry;
task->param = param;
task->status = PENDING;
task->result = NULL;
task->id = id++;
return (task);
}

/**
 * destroy_task - destroys a task
 * @task: pointer to the task to destroy
 * Return: Nothing
 */
void destroy_task(task_t *task)
{
if (task)
{
list_destroy(task->result, free);
if (task->result)
free(task->result);
free(task);
}
}

/**
 * exec_tasks - executes list of tasks
 * @tasks: pointer to list of tasks to be executed
 * Return: Nothing
 */
void *exec_tasks(list_t const *tasks)
{
int tpending = 1;
int tid;
node_t *node;
if (!tasks)
pthread_exit(NULL);
while (tpending)
{
for (tpending = 0, node = tasks->head; node; node = node->next)
{
if (task_stat(node->content) == PENDING)
{
tpending = 1;
tid = ((task_t *)node->content)->id;
set_task_stat(node->content, STARTED);
tprintf("[%02d] Started\n", tid);
if (exec_task(node->content))
{
set_task_stat(node->content, SUCCESS);
tprintf("[%02d] Success\n", tid);
}
else
{
set_task_stat(node->content, FAILURE);
tprintf("[%02d] Failure\n", tid);
}
}
}
}
return (NULL);
}
