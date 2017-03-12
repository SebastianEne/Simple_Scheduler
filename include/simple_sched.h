#ifndef __SIMPLE_SCHED
#define __SIMPLE_SCHED

#include "task.h"

void scheduler_initialize(void);

struct task_s *scheduler_get_next_context(void);

void scheduler_do_run(void);

void scheduler_add_task(struct task_s *task);

void scheduler_remove_task(struct task_s *task);

void scheduler_destroy(void);

#endif //__SIMPLE_SCHED
