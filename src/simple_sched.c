#include <stdio.h>
#include <time.h>
#include <semaphore.h>

#include "simple_sched.h"


#define DISABLE_INT             sem_wait(&g_sem_init);

#define ENABLE_INT              sem_post(&g_sem_init);

#define NUM_TASKS               2

static void t1(void);

static void t2(void);


static sem_t   g_sem_init;

static struct task_s running_task[NUM_TASKS];

static struct task_s current_task;

static int task_index;


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

/* Build and initialized a new scheduler create tasks
 * list. */
void scheduler_initialize(void)
{
    int ret;
    char t1_stack[2048];
    char t2_stack[2048];

    ret = getcontext(&running_task[0].task_context);
    if (ret < 0)
      {
        fprintf(stderr, "Cannot get context\n");
        return;
      }

    running_task[0].task_context.uc_stack.ss_sp = t1_stack;
    running_task[0].task_context.uc_stack.ss_size = sizeof(t1_stack);
    running_task[0].entry_point = t1;

    makecontext(&running_task[0].task_context, t1, 0);

    ret = getcontext(&running_task[1].task_context);
    if (ret < 0)
      {
        fprintf(stderr, "Cannot get context\n");
        return;
      }

    running_task[1].task_context.uc_stack.ss_sp = t2_stack;
    running_task[1].task_context.uc_stack.ss_size = sizeof(t2_stack);
    running_task[1].entry_point = t2;

    makecontext(&running_task[1].task_context, t2, 0);

    current_task.entry_point = running_task[0].entry_point;

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
  task_index++;
  task_index = task_index % NUM_TASKS;

  return &running_task[task_index];
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
