//
// Created by corentin on 30/01/24.
//

#include "headers/process.h"
#define PATH_MAX 1000
#define MAX_LINE_LENGTH 256
#define STATUS_FILE "/proc/%s/status"
#include <dirent.h>
#include <ctype.h>
#include <string.h>

void printProcessName(const char *pid)
{
    FILE *file;
    char filename[MAX_LINE_LENGTH];
    char line[MAX_LINE_LENGTH];

    snprintf(filename, sizeof (filename), STATUS_FILE, pid);

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening status file");
        return;
    }

    while(fgets(line, sizeof(line), file))
    {
        if(strncmp(line, "Name:", 5) == 0)
        {
            printf("PID: %s, Name: %s", pid, line + 6);
            break;
        }
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
    while ((entry = readdir(dir)) != NULL)
    {
        if( entry->d_type == DT_DIR && isdigit( entry->d_name[0] ) ) {
            printProcessName(entry->d_name);
        }
    }
    closedir(dir);
}

