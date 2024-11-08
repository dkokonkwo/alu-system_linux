#include "multithreading.h"


__attribute__((constructor)) void task_mutex_init(void)
{
pthread_mutex_init(&task_mutex, NULL);
}

__attribute__((destructor)) void task_mutex_destroy(void)
{
pthread_mutex_destroy(&task_mutex);
}

void end(void) __attribute__((destructor));


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
free(task->result);
free(task);
}
}

/**
 * exec_task - executes list of tasks
 * @tasks: pointer to list of tasks to be executed
 * Return: Nothing
 */
void *exec_tasks(list_t const *tasks)
{
int tpending = 1, tid;
node_t *node;
if (!tasks)
pthread_exit(NULL);
while (tpending)
{
tpending = 0;
for (node = tasks->head; node; node = node->next)
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
tprintf("[%02d] Succees\n", tid);
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

/**
 * exec_task - executes task in node
 * @task: pointer to task to execute
 * Return: result
 */
void *exec_task(task_t *task)
{
void *result;
result = task->entry(task->param);
pthread_mutex_lock(&task->lock);
task->result = result;
pthread_mutex_unlock(&task->lock);
return (result);
}

/**
 * task_stat - gets task's status
 * @task: pointer to task
 * Return: task status
 */
task_status_t task_stat(task_t *task)
{
task_status_t stat;
pthread_mutex_lock(&task->lock);
stat = task->status;
pthread_mutex_unlock(&task->lock);
return (stat);
}

/**
 * set_task_stat - sets task's status
 * @task: pointer to task
 * @stat: new status to set
 * Return: Nothing
 */
void set_task_stat(task_t *task, task_status_t stat)
{
pthread_mutex_lock(&task->lock);
task->status = stat;
pthread_mutex_unlock(&task->lock);
}
