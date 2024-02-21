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
#include "utils.h"



typedef struct {
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

typedef struct  {
    long mem_total;
    long mem_free;
    long buffers;
    long cached;
    long swap_total;
    long swap_free;
}MemoryStat;

float processMemUsage(const char *pid);
MemoryStat memUsage();
float getUptime();
int getNumberCpu();
void getCpuUsage(CPUStats* stats);
void calculateCPUUsage(double *usage);
void cpuUsage(WINDOW *boite);
void findProcess(WINDOW *boite, int indexscroll);
void printProcessDetails(const char *pid, WINDOW *boite, int i, int indexscroll);

#endif //HTOP_PROCESS_H