#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <assert.h>
#include <string.h>

#include "simple_sched.h"
#include "queue.h"


#define DISABLE_INT             sem_wait(&g_sem_init);

#define ENABLE_INT              sem_post(&g_sem_init);


static sem_t   g_sem_init;

static struct queue_s *pending_tasks;

static struct task_s current_task;

static void t1(const struct task_s * const task)
{

  while (1)
    {
      printf("[task %d] runs !\n", task->pid);
      while (1)
        {
          ;;
        }
    }
}

static void build_demo_tasks(int num_tasks)
{
  struct task_s **task;
  int i;

  assert(num_tasks >= 0);

  task = (struct task_s **) malloc(sizeof(struct task_s) * num_tasks);
  if (task == NULL)
    {
      fprintf(stderr, "ERROR: Cannot build demo tasks\n");
      return;
    }

  for (i = 0; i < num_tasks; ++i)
    {
      task[i] = create_task(1, 2048, t1);
      pending_tasks = enqueue(pending_tasks, task[i], sizeof(struct task_s));
    }

  current_task = *task[0];
}


/* Build and initialized a new scheduler create tasks
 * list. */
void scheduler_initialize(void)
{
    int ret;

    build_demo_tasks(10);

    ret = sem_init(&g_sem_init, 0, 1);
    if (ret < 0)
        {
          fprintf(stderr, "Cannot build semaphore");
          return;
        }
}

/* Adds a new task to the task list. This should disable
 * interrupts before running. */
void scheduler_add_task(struct task_s *task)
{
    DISABLE_INT;

    ENABLE_INT;
}

struct task_s *scheduler_get_next_context(void)
{
  pending_tasks = rotate(pending_tasks);
  return (struct task_s *)pending_tasks->data;
}

/* Removes a new task from the task list. This should disable
 * interrupts before running. */
void scheduler_remove_task(struct task_s *task)
{
    DISABLE_INT;

    ENABLE_INT;
}

/* Iterate through the tasks queues and schedule the
 * task with the highest priority. This should be done
 * on interrupt context. */
void scheduler_do_run(void)
{
    DISABLE_INT;

    ((t)current_task.entry_point)(&current_task);

    ENABLE_INT;
}

/* Release tasks memory and stop scheduler. */
void scheduler_destroy(void)
{
    int ret;

    ret = sem_destroy(&g_sem_init);
    if (ret < 0)
        {
            fprintf(stderr, "Cannot relase semaphore\n");
            return;
        }
}
