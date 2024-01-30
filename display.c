//
// Created by corentin on 30/01/24.
//

#include "headers/display.h"
#include <stdio.h>

void ncursing()
{
    initscr();
    move(LINES - 1, COLS -1);
    addch('.');

    getch();
    endwin();
}