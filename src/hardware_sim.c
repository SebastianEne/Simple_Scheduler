#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#include "simple_sched.h"

#if __linux__

#define SIGINFO     SA_SIGINFO

#define REG_ORIG_R0	17
#define REG_CPSR	16
#define REG_PC		15
#define REG_LR		14
#define REG_SP		13
#define REG_IP		12
#define REG_FP		11
#define REG_R10		10
#define REG_R9		9
#define REG_R9		9
#define REG_R8		8
#define REG_R7		7
#define REG_R6		6
#define REG_R5		5
#define REG_R4		4
#define REG_R3		3
#define REG_R2		2
#define REG_R1		1
#define REG_R0		0

#endif


/* Private types definitions. */

struct rtc_timer
{
    clock_t last_time;
    int rtc_period;
};


/* Private function definitions. */

static void rtc_sim(void *not_used);

static void rtc_timer_create(clock_t current, int rtc_period_milis);

static void rtc_interrupt(void);

static void sig_handler(int sig_type, siginfo_t *info, void *context);

static void sig_restore(void);


/* Private variables. */

static pthread_t rtc_thread;

static pid_t current_pid;

struct rtc_timer timsk = {
    .last_time  = 0,
    .rtc_period = 0
};

struct sigaction rtc_action = {
#if __APPLE__
    .__sigaction_u.__sa_sigaction   = sig_handler,
#elif __linux__
    .sa_sigaction                   = sig_handler,
#endif
    .sa_flags        = SA_SIGINFO,
};


/* Public variables. */

volatile int g_cpu_active;


/* Private function implementation. */
static void sig_handler(int sig_type, siginfo_t *info, void *context)
{
   printf("[INT] RTC interrupt\n");

   ucontext_t *task_context = (ucontext_t *) context;

   for (int i = 0; i < 17; ++i)
     {
       task_context->uc_mcontext.gregs[i] =
         scheduler_get_next_context()->task_context.uc_mcontext.gregs[i];
     }
}

static void rtc_sim(void *not_used)
{
    while (g_cpu_active)
        {
           clock_t new_time = clock();

           if ((new_time - timsk.last_time) * 1000 / CLOCKS_PER_SEC >
                    timsk.rtc_period)
                {
                    timsk.last_time = new_time;
                    rtc_interrupt();
                }
        }
}

static void rtc_timer_create(clock_t current, int rtc_period_milis)
{
    timsk.last_time     = current;
    timsk.rtc_period    = rtc_period_milis;
}

static void rtc_interrupt(void)
{
    int ret;

    printf("[INT] send signal to interrupt main thread\n");

    ret = kill(current_pid, SIGINFO);
    if (ret < 0)
        {
            fprintf(stderr, "Signal not sent, abort\n");
            g_cpu_active = 0;
        }
}

void sig_handler_cancel(int signo)
{
    g_cpu_active = 0;
    fprintf(stdout, "\n[Halt] Simulation canceled\n");
    exit(1);
}


/* Public function implementation. */

void hardware_init(void)
{
    int ret;

    g_cpu_active = 1;

    rtc_timer_create(clock(), 1000);

    if (signal(SIGINT, sig_handler_cancel) == SIG_ERR)
        printf("\ncan't catch SIGSTOP\n");

    ret = sigaction(SIGINFO, &rtc_action, NULL);
    if (ret < 0)
        {
            fprintf(stderr, "Cannot install signal handler\n");
            exit(-1);
        }

    scheduler_initialize();
    current_pid = getpid();

    ret = pthread_create(&rtc_thread, NULL, (void *(*)(void *))rtc_sim, NULL);
    if (ret < 0)
        {
            fprintf(stderr, "Cannot create thread\n");
            exit(-1);
        }
}

void hardware_run(void)
{
    while(g_cpu_active)
        {
            scheduler_do_run();
        }
}

void hardware_suspend(void)
{
    int ret;

    fprintf(stdout, "\n[CPU] hardware power off !\n");

    ret = pthread_join(rtc_thread, NULL);
    if (ret < 0)
        {
            fprintf(stderr, "Cannot join thread\n");
            exit(-1);
        }
}
