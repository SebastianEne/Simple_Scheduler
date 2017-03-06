#include <stdio.h>
#include <time.h>
#include <semaphore.h>

#ifdef __APPLE__
    #define get_errno()     0
#else 
    #include <errno.h>
#endif

#include "simple_sched.h"


#define DISABLE_INT             sem_wait(&g_sem_init);

#define ENABLE_INT              sem_post(&g_sem_init);


sem_t   g_sem_init;


/* Build and initialized a new scheduler create tasks
 * list. */
void scheduler_initialize(void)
{
    int ret;

    ret = sem_init(&g_sem_init, 0, 1);
    if (ret < 0)
        {
            fprintf(stderr, "Cannot build semaphore:%d\n", get_errno());
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

    ENABLE_INT;
}

/* Release tasks memory and stop scheduler. */
void scheduler_destroy(void)
{
    int ret;

    ret = sem_destroy(&g_sem_init);
    if (ret < 0)
        {
            fprintf(stderr, "Cannot build semaphore:%d\n", get_errno());
            return;
        }
}
