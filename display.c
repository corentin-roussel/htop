//
// Created by corentin on 30/01/24.
//

#include "headers/display.h"

void ncursing()
{
    WINDOW *boite = initscr();
    scrollok(boite, TRUE);
    noecho();
    keypad(stdscr, TRUE);

    findProcess();
    refresh();
    while(1) {

        if(getch() == KEY_UP)
        {
            findProcess();
            refresh();
        }
        if(getch() == 'q' || getch() == 'Q')
        {
            break;
        }
    }

    endwin();
}