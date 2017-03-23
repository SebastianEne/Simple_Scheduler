#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "task.h"


static int task_num;

struct task_s *create_task(int priority, ssize_t stack_size, t entry_point)
{
  struct task_s *task;
  int ret;
  char task_name[20];
  char *argv[2];

  assert(entry_point != NULL);

  task = (struct task_s *) malloc(sizeof(struct task_s));
  if (task == NULL)
    {
      fprintf(stderr, "Cannot allocate task structure");
      return task;
    }

  task->task_state  = INACTIVE;
  task->priority    = priority;
  task->stack_size  = stack_size;
  task->entry_point = entry_point;
  task->pid         = task_num++;

  task->stack = malloc(task->stack_size);
  if (task->stack == NULL)
    {
      free(task);
      task = NULL;
      return task;
    }

  ret = getcontext(&task->task_context);
  if (ret < 0)
    {
      fprintf(stderr, "Cannot get context\n");
      destroy_task(task);
      return NULL;
    }

  task->task_context.uc_stack.ss_sp   = task->stack;
  task->task_context.uc_stack.ss_size = task->stack_size;

  memset(task_name, 0, sizeof(task_name));
  snprintf(task_name, sizeof(task_name), "task %d", task->pid);
  *argv = task_name;

  makecontext(&task->task_context, task->entry_point, 1, task);

  return task;
}

void destroy_task(struct task_s *task)
{
  assert(task != NULL);

  free(task->stack);
  task->stack = NULL;

  free(task);
  task = NULL;
}

void suspend_task(struct task_s *task)
{
  // TODO
}

void resume_task(struct task_s *task)
{
  // TODO
}


