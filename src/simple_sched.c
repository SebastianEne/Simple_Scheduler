#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <assert.h>

#include "simple_sched.h"
#include "queue.h"


#define DISABLE_INT             sem_wait(&g_sem_init);

#define ENABLE_INT              sem_post(&g_sem_init);


static sem_t   g_sem_init;

static struct queue_s *pending_tasks;

static struct task_s current_task;

static volatile int num_tasks;


static void t1(void)
{
  while (1)
    {
      printf("[t1] runs !\n");
      sleep(1);
    }
}

static void t2(void)
{
  while (1)
    {
      printf("[t2] runs !\n");
      sleep(1);
    }
}

static void build_demo_tasks(int num_tasks)
{
  struct task_s *task_1, *task_2;

  assert(num_tasks >= 0);

  task_1 = create_task(1, 2048, t1);
  pending_tasks = enqueue(pending_tasks, task_1, sizeof(struct task_s));

  task_2 = create_task(1, 2048, t2);
  pending_tasks = enqueue(pending_tasks, task_2, sizeof(struct task_s));

  current_task = *task_1;
}


/* Build and initialized a new scheduler create tasks
 * list. */
void scheduler_initialize(void)
{
    int ret;

    build_demo_tasks(2);

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

    current_task.entry_point();

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
