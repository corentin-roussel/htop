//
// Created by corentin on 30/01/24.
//

#ifndef HTOP_PROCESS_H
#define HTOP_PROCESS_H
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "display.h"
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h>

void findProcess();
void printProcessName(const char *pid);

#endif //HTOP_PROCESS_H