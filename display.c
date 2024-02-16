//
// Created by corentin on 30/01/24.
//

#include "headers/display.h"
#define NUM_STRINGS 100

void ncursing()
{
    int start_row = 1;
    int ch;

    initscr();
    WINDOW *cpu_window = newwin(0,100,0,0);
    WINDOW *boite = newwin(25, 100, 20, 0);
    cbreak();
    noecho();
    box(cpu_window, 0, 0);
    box(boite, 0, 0);
    wrefresh(boite);
    wrefresh(cpu_window);

    scrollok(boite, TRUE);
    scrollok(cpu_window, TRUE);
    keypad(stdscr, TRUE);




    while ((ch = getch()) != 'q') {
        wclear(boite);
        wclear(cpu_window);
        findProcess(boite, start_row);
        cpuUsage(cpu_window);
        wrefresh(boite);
        wclear(cpu_window);
        if (ch == KEY_UP) {
            // Handle scrolling up
            if (start_row >= 1) {
                wclear(boite);
                start_row--; // Decrement start row
                findProcess(boite, start_row);
                wrefresh(boite);
            }
        } else if (ch == KEY_DOWN) {
            wclear(boite);
            start_row++; // Decrement start row
            findProcess(boite, start_row);
            wrefresh(boite);
        }
        if(ch == ' ')
        {
            wclear(boite);
            wclear(cpu_window);
            findProcess(boite, start_row);
            cpuUsage(cpu_window);
            wrefresh(boite);
            wrefresh(cpu_window);
        }
        refresh();
    }

    endwin();
}