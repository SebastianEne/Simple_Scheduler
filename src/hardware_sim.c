#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

/* Private types definitions. */

struct rtc_timer 
{
    clock_t last_time;
    int rtc_period;
};    

/* Private function definitions. */

static void rtc_sim(void *not_used);

static void timer_create(clock_t current, int rtc_period_milis);

static void rtc_interrupt(void);

static void sig_handler(int sig_type, siginfo_t *info, void *context);


/* Private variables. */

static pthread_t rtc_thread;

static pid_t current_pid;

struct rtc_timer timsk = {
    .last_time  = 0,
    .rtc_period = 0
};

struct sigaction rtc_action = {
    .__sigaction_u   = sig_handler,
    .sa_flags        = SA_SIGINFO
};

/* Public variables. */

volatile int g_cpu_active;


/* Private function implementation. */
static void sig_handler(int sig_type, siginfo_t *info, void *context)
{
   printf("Called signal handler %p!\n",pthread_self()); 
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

static void timer_create(clock_t current, int rtc_period_milis)
{
    timsk.last_time     = current;
    timsk.rtc_period    = rtc_period_milis;
}

static void rtc_interrupt(void)
{
    int ret;

    printf("[INT]   RTC_CLOCK send signal to interrupt main thread %p\n", pthread_self());
    
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
}

/* Public function implementation. */

void hardware_init(void)
{
    int ret;

    g_cpu_active = 1;

    timer_create(clock(), 1000);
    
    if (signal(SIGINT, sig_handler_cancel) == SIG_ERR)
        printf("\ncan't catch SIGSTOP\n");
 
    ret = sigaction(SIGINFO, &rtc_action, NULL);
    if (ret < 0)
        {
            fprintf(stderr, "Cannot install signal handler\n");
            exit(-1);
        }

    current_pid = getpid();

    ret = pthread_create(&rtc_thread, NULL, rtc_sim, NULL);
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
        }
}

void hardware_suspend(void)
{
    int ret;

    fprintf(stdout, "\n[CPU] hardware power off !\n");

    ret = pthread_join(&rtc_thread, NULL);
    if (ret < 0)
        {
            fprintf(stderr, "Cannot join thread\n");
            exit(-1);
        }
}
