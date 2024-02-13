//
// Created by corentin on 30/01/24.
//

#include "headers/process.h"
#define MAX_PATH 1000
#define MAX_LINE_LENGTH 256
#define BUF_MAX 1024
#define STAT_FILE "/proc/%s/stat"
#define PROC_STAT "/proc/stat"
#define MEMORY "/proc/meminfo"

struct ProcessStat {
    int pid;
    char *comm;
    char state;
    char *vmpeak;
    char *pgrp;
} ProcessStat;




int my_atoi(char *str)
{
    int i = 0;
    int ascii = 0;

    while(str[i] != '\0')
    {
        ascii = ascii * 10 + (str[i] - '0');
        ++i;
    }
    return ascii;
}

int getNumberCpu()
{
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        perror("Error opening /proc/cpuinfo");
        exit(1);
    }

    int num_cpus = 0;
    char line[BUF_MAX];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "processor", 9) == 0) {
            num_cpus++;
        }
    }
    fclose(fp);
    return num_cpus;
}

void getCpuUsage(CPUStats* stats) {
    FILE* file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Error opening /proc/stat");
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_MAX];
    while (fgets(buffer, BUF_MAX, file)) {
        if (strncmp(buffer, "cpu", 3) == 0) {
            sscanf(buffer + 5, "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
                   &stats->user, &stats->nice, &stats->system, &stats->idle,
                   &stats->iowait, &stats->irq, &stats->softirq, &stats->steal,
                   &stats->guest, &stats->guest_nice);
            break;
        }
    }

    fclose(file);
}

void calculateCPUUsage(double *usage) {
    int MAX_CPUS = getNumberCpu();
    CPUStats lastStats[MAX_CPUS];
    static int numCPUs = 0;

    CPUStats currentStats[MAX_CPUS];
    getCpuUsage(currentStats);

    if (numCPUs == 0) {
        for (int i = 0; i < MAX_CPUS; ++i) {
            if (currentStats[i].user == 0 && currentStats[i].nice == 0 && currentStats[i].system == 0 &&
                currentStats[i].idle == 0 && currentStats[i].iowait == 0 && currentStats[i].irq == 0 &&
                currentStats[i].softirq == 0 && currentStats[i].steal == 0 && currentStats[i].guest == 0 &&
                currentStats[i].guest_nice == 0) {
                break;
            }
            ++numCPUs;
        }
    }


    for (int i = 0; i < numCPUs; ++i) {
        unsigned long totalDiff = (currentStats[i].user + currentStats[i].nice + currentStats[i].system +
                                   currentStats[i].idle + currentStats[i].iowait + currentStats[i].irq +
                                   currentStats[i].softirq + currentStats[i].steal) -
                                  (lastStats[i].user + lastStats[i].nice + lastStats[i].system +
                                   lastStats[i].idle + lastStats[i].iowait + lastStats[i].irq +
                                   lastStats[i].softirq + lastStats[i].steal);
        unsigned long idleDiff = (currentStats[i].idle - lastStats[i].idle);

        usage[i] = 100*(totalDiff - idleDiff)  / totalDiff;

        // Update lastStats
        lastStats[i] = currentStats[i];
    }
}

void cpuUsage()
{

    int num_cpu = getNumberCpu();

    double cpu_usage[num_cpu + 1];
    calculateCPUUsage(cpu_usage);

    for(int i = 0; i < num_cpu; i++)
    {
        printf("CPU %d Usage: %.2f%%\n", i, cpu_usage[i]);
    }
}

struct MemoryStat memUsage()
{
    FILE *memory_file = fopen(MEMORY, "r");

    if(memory_file == NULL)
    {
        perror("Error opening /proc/stat");
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_MAX];

    while(fgets(buffer, sizeof(buffer),memory_file))
    {


        char *token  = strtok(buffer, " ");
        if(strncmp(buffer, "MemTotal:",  9) == 0)
        {
            token = strtok(NULL, " ");
            while( token != NULL)
            {
                float value = strtol(token, NULL, 10);
                memoryStat.mem_total = value/ 1048576;
                break;
            }
        }
        if(strncmp(buffer, "MemFree:",  8) == 0)
        {
            token = strtok(NULL, " ");
            while( token != NULL)
            {
                float value = strtol(token, NULL, 10);
                memoryStat.mem_free = value/ 1048576;
                break;
            }
        }
        if(strncmp(buffer, "SwapTotal:",  10) == 0)
        {
            token = strtok(NULL, " ");
            while( token != NULL)
            {
                float value = strtol(token, NULL, 10);
                memoryStat.swap_total = value/ 1048576;
                break;
            }
        }
        if(strncmp(buffer, "SwapFree:",  9) == 0)
        {
            token = strtok(NULL, " ");
            while( token != NULL)
            {
                float value = strtol(token, NULL, 10);
                memoryStat.swap_free = value/ 1048576;
                break;
            }
        }
    }
    return memoryStat;

}

void printProcessDetails(const char *pid)
{
    FILE *file;
    char filename[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    char *tok;
    int index = 0;
    struct ProcessStat process = { 0, "", ' ', 0, 0};

    snprintf(filename, sizeof (filename), STAT_FILE, pid);

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening status file");
        return;
    }

    while(fgets(line, sizeof(line), file))
    {
//        if(strncmp(line, "Name:", 5) == 0)
//        {
//            printw("%s\t %s", pid, line + 6);
//            break;
//        }
        while((tok = strtok(line, " ")) != NULL)
        {
            index++;
            if(index == 2)
            {
                strcpy(process.comm, tok);
                strcat(process.comm, " ");

            }
            if(index == 14)
            {
                strcpy(process.vmpeak, tok);
                strcat(process.comm, " ");
            }
        }
        tok = strtok(NULL, " ");

        printw("%s \t %s \t %s",pid, process.comm, process.vmpeak);

    }
    fclose(file);
}

void findProcess()
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX] = "/proc";

    dir = opendir(path);

    if(dir == NULL)
    {
        perror("\n The file could "
               "not be opened");
        exit(1);
    }
    printw("PID\t Name\n");
    while ((entry = readdir(dir)) != NULL)
    {
        if( entry->d_type == DT_DIR && isdigit( entry->d_name[0] ) ) {
            printProcessDetails(entry->d_name);
        }
    }
    closedir(dir);
}

