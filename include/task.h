#ifndef __TASK_H
#define __TASK_H
#define _XOPEN_SOURCE

#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <ucontext.h>

#define TASK_SIZE           2048


enum task_state_e
{
    INACTIVE = 0,
    PENDING,
    ACTIVE,
};

typedef enum task_state_e task_state_t;

struct task_s {
    task_state_t    task_state;     // The state of the current task
    ucontext_t      task_context;   // The task context
    int             priority;       // The task's priority. (0 - highest)
    void           *stack;          // The task's stack address.
    ssize_t         stack_size;     // The size of the stack.
    void           *entry_point;    // The task's entry point.
    int             pid;            // The rask pid.
};

typedef void (*t)(const struct task_s * const task);


struct task_s *create_task(int priority, ssize_t stack_size, t entry_point);

void destroy_task(struct task_s *task);

void suspend_task(struct task_s *task);

void resume_task(struct task_s *task);

#endif // __TASK_H
