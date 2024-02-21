//
// Created by corentin on 30/01/24.
//

#include "headers/process.h"
#define MAX_PATH 1000
#define MAX_LINE_LENGTH 1024
#define BUF_MAX 1024
#define STAT_FILE "/proc/%s/stat"
#define STATUS_FILE "/proc/%s/status"
#define PROC_STAT "/proc/stat"
#define MEMORY "/proc/meminfo"
#define UPTIME "/proc/uptime"


struct ProcessStat {
    int pid;
    char *comm;
    char *state;
    float cpu_usage;
    int vmrss;
    int pgrp;
    struct ProcessStat * next;
} processStat;






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

void cpuUsage(WINDOW *boite)
{

    int num_cpu = getNumberCpu();

    double cpu_usage[num_cpu + 1];
    calculateCPUUsage(cpu_usage);
    MemoryStat memoryUsage = memUsage();

        mvwprintw(boite, 1, 1,"CPU %d Usage: %.2f%%\n", 1, cpu_usage[0]);
        mvwprintw(boite, 1, 25,"CPU %d Usage: %.2f%%\n", 2, cpu_usage[1]);
        mvwprintw(boite, 2, 1,"CPU %d Usage: %.2f%%\n", 3, cpu_usage[2]);
        mvwprintw(boite, 2, 25,"CPU %d Usage: %.2f%%\n", 4, cpu_usage[3]);
        mvwprintw(boite, 3, 1,"CPU %d Usage: %.2f%%\n", 5, cpu_usage[4]);
        mvwprintw(boite, 3, 25,"CPU %d Usage: %.2f%%\n", 6, cpu_usage[5]);
        mvwprintw(boite, 4, 1,"CPU %d Usage: %.2f%%\n", 7, cpu_usage[6]);

    mvwprintw(boite, 5,1, "MemUsage %.2lf", ((double)(memoryUsage.mem_total - memoryUsage.mem_free - memoryUsage.buffers - memoryUsage.cached) / memoryUsage.mem_total) * 100);
    mvwprintw(boite, 6, 1, "SwapUsage %.2lf",((double)(memoryUsage.swap_total - memoryUsage.swap_free) / memoryUsage.swap_total) * 100);
}

MemoryStat memUsage()
{
    FILE *memory_file = fopen(MEMORY, "r");

    MemoryStat memoryStat = {0};

    if(memory_file == NULL)
    {
        perror("Error opening /proc/stat");
        exit(EXIT_FAILURE);
    }

    char line[BUF_MAX];

    while(fgets(line, sizeof(line),memory_file) != NULL)
    {
        if (strstr(line, "MemTotal:") == line) {
            sscanf(line, "MemTotal: %ld kB", &memoryStat.mem_total);
        } else if (strstr(line, "MemFree:") == line) {
            sscanf(line, "MemFree: %ld kB", &memoryStat.mem_free);
        } else if (strstr(line, "Buffers:") == line) {
            sscanf(line, "Buffers: %ld kB", &memoryStat.buffers);
        } else if (strstr(line, "Cached:") == line) {
            sscanf(line, "Cached: %ld kB", &memoryStat.cached);
        } else if (strstr(line, "SwapTotal:") == line) {
            sscanf(line, "SwapTotal: %ld kB", &memoryStat.swap_total);
        } else if (strstr(line, "SwapFree:") == line) {
            sscanf(line, "SwapFree: %ld kB", &memoryStat.swap_free);
        }
    }
    fclose(memory_file);
    return memoryStat;
}

float getUptime()
{
    FILE *uptime = fopen(UPTIME, "r");

    char time[MAX_LINE_LENGTH];
    if(uptime == NULL)
    {
        perror("Error opening status file");
        return 0;
    }

    fgets(time, sizeof time, uptime);

    char *tok = strtok(time, " ");
    fclose(uptime);
    return atoi(tok);
}

float processMemUsage(const char *pid) {

    FILE *file;
    char filename[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    int memory_usage = 0;

    snprintf(filename, sizeof(filename), STATUS_FILE, pid);

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening status file");
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            sscanf(line, "VmRSS: %d", &memory_usage);
            break;
        }
    }
    float memory_usage_gb = (float)memory_usage /( (1024.0 * 1024.0));
    fclose(file);
    return memory_usage_gb;
}


void printProcessDetails(const char *pid, WINDOW *boite, int i, int indexscroll)
{
    FILE *file;
    char filename[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];
    char **splitted;
    unsigned long utime;
    unsigned long stime;
    long int cutime;
    long int cstime;
    long int starttime;
    long int total_time;
    float seconds;
    int hertz = sysconf(_SC_CLK_TCK);

    float uptime = getUptime();



    snprintf(filename, sizeof (filename), STAT_FILE, pid);
    float memUsage = processMemUsage(pid);

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening status file");
        return;
    }

    while(fgets(line, sizeof(line), file) != NULL)
    {
        splitted = split(line);

        utime = atoi(splitted[13]);
        stime = atoi(splitted[14]);
        cutime = atoi(splitted[15]);
        cstime = atoi(splitted[16]);
        total_time = utime +stime + cutime + cstime;
        starttime = atoi(splitted[21]);
        processStat.comm = splitted[1];
        seconds = uptime-(starttime / hertz);
//        printf("%d, %lu\n", hertz, starttime);
        processStat.cpu_usage = 100 * ((total_time / hertz) /seconds);
        processStat.pid = atoi(splitted[0]);
        processStat.state = splitted[2];
//        printf("%lu\n", processStat.cpu_usage);

        if(i > indexscroll)
        {
            mvwprintw(boite, i - indexscroll, 1, "%s",pid);
            mvwprintw(boite, i - indexscroll, 10, "%s", processStat.state);
            mvwprintw(boite, i - indexscroll, 20, "%.2f%c",  processStat.cpu_usage, 37);
            mvwprintw(boite, i - indexscroll, 30, "%.2fGB", memUsage);
            mvwprintw(boite, i - indexscroll, 50, "%s", processStat.comm);
        }
    }
    fclose(file);


}

void findProcess(WINDOW *boite, int indexscroll)
{
    DIR *dir;
    struct dirent *entry;
    int index = 1;
    char path[PATH_MAX] = "/proc";

    dir = opendir(path);

    if(dir == NULL)
    {
        perror("\n The file could "
               "not be opened");
        exit(1);

    }

    mvwprintw(boite, 0, 1, "PID");
    mvwprintw(boite, 0, 50, "Name");
    mvwprintw(boite, 0, 10, "State");
    mvwprintw(boite, 0, 20, "CPU%c", 37);
    mvwprintw(boite, 0, 30, "Mem GB");
    while ((entry = readdir(dir)) != NULL)
    {
        if( entry->d_type == DT_DIR && isdigit( entry->d_name[0] ) ) {
            printProcessDetails(entry->d_name,  boite, index, indexscroll);
            index++;
        }
    }
    closedir(dir);
}

