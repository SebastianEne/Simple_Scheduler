#include <stdio.h>
#include "hardware_sim.h"


int main(int argc, char **argv)
{
    hardware_init();       
    hardware_run();
    hardware_suspend();

    return 0;
}

