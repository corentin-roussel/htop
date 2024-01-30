//
// Created by corentin on 30/01/24.
//

#include "headers/process.h"


int findProcess()
{
    int command  = system("cat /proc/meminfo");

    return command;
}