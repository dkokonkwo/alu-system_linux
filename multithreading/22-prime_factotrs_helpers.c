#include "multithreading.h"

/**
 * exec_task - executes task in node
 * @task: pointer to task to execute
 * Return: result
 */
void *exec_task(task_t *task)
{
void *result = task->entry(task->param);
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
