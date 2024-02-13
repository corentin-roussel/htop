//
// Created by corentin on 30/01/24.
//

#ifndef HTOP_PROCESS_H
#define HTOP_PROCESS_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "main.h"
#include "display.h"
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h>

typedef struct statistics{
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long iowait;
    unsigned long irq;
    unsigned long softirq;
    unsigned long steal;
    unsigned long guest;
    unsigned long guest_nice;
} CPUStats;

struct MemoryStat {
    float mem_total;
    float mem_free;
    float swap_total;
    float swap_free;
} memoryStat;

struct MemoryStat memUsage();
int getNumberCpu();
void getCpuUsage(CPUStats* stats);
void calculateCPUUsage(double *usage);
void cpuUsage();
void findProcess();
void printProcessDetails(const char *pid);

#endif //HTOP_PROCESS_H