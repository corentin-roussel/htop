//
// Created by corentin on 30/01/24.
//

#include "headers/display.h"
#define NUM_STRINGS 100

void ncursing()
{
    int start_row = 1;


    initscr();
    WINDOW *cpu_window = newwin(0,100,0,0);
    WINDOW *boite = newwin(55, 100, 10, 0);
    noecho();



    scrollok(boite, TRUE);
    keypad(stdscr, TRUE);

    timeout(2000);


    findProcess(boite, start_row);
    cpuUsage(cpu_window);
    while (1) {
        int ch = getch();
        if(ch == ERR)
        {
            findProcess(boite, start_row);
            cpuUsage(cpu_window);
        }else {

            if (ch == KEY_UP) {
                // Handle scrolling up
                if (start_row >= 1) {
                    wclear(boite);
                    start_row--; // Decrement start row
                    findProcess(boite, start_row);
                }

            } else if (ch == KEY_DOWN) {
                wclear(boite);
                start_row++; // Increment start row
                findProcess(boite, start_row);
            }else if(ch == 'q')
            {
                break;
            }else if(ch == ' ')
            {
                findProcess(boite, start_row);
                cpuUsage(cpu_window);
            }
        }
        wrefresh(boite);
        wrefresh(cpu_window);
        refresh();
    }
    endwin();
}

void progressBar(float percentage, int placeX, int placeY)
{
    int width = 45; // Width of the percentage bar


    // Main loop
    while (percentage <= 100) {

        // Calculate number of characters to represent the progress
        int progress = (int)(percentage / 100 * width);

        // Print the percentage bar
        mvprintw(placeX, placeY+50, "%.2f%%", percentage);
        mvprintw( placeX, placeY+1, "[");
        for (int i = 0; i < width; i++) {
            if(i < progress){
                addch('=');
            }
            else {
                addch(' ');
            }
        }

        addch(']');

        // Increment percentage
        percentage += 1.0;
    }
}