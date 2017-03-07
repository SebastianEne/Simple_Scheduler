#ifndef __TASK_H
#define __TASK_H
#define _XOPEN_SOURCE

#include <stdint.h>
#include <ucontext.h>

#define TASK_SIZE           2048


enum task_state_e
{
    INACTIVE = 0,
    PENDING,
    ACTIVE,
};

typedef enum task_state_e task_state_t;

struct cpu_s
{
    uint64_t RAX;   // Register R0 extended
    uint64_t RBX;   // Register R1 extended
    uint64_t RCX;   // Register R2 extended
    uint64_t RDX;   // Register R3 extended
    uint64_t RBP;   // Base pointer register
    uint64_t RSP;   // Stack pointer register
    uint64_t RSI;   // Source index register
    uint64_t RDI;   // Dest index register
    uint64_t R8;
    uint64_t R9;
    uint64_t R10;
    uint64_t R11;
    uint64_t R12;
    uint64_t R13;
    uint64_t R14;
    uint64_t R15;
};

typedef struct cpu_s cpu_t;

struct task_s {
    cpu_t           state;          // CPU state with all registers
    task_state_t    task_state;     // The state of the current task
    ucontext_t      task_context;   // The task context
    int             priority;       // The task's priority. (0 - highest)
};


struct task_s *create_task(void);

void destroy_task(struct task_s *task);

void suspend_task(struct task_s *task);

void resume_task(struct task_s *task);

#endif // __TASK_H
