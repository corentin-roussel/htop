//
// Created by corentin on 30/01/24.
//

#include "headers/display.h"
#include <stdio.h>
#include <stdlib.h>

int ncursing()
{
    while(1) {
        refresh();
        findProcess();
        if(getch() != 'q' )  // 410 est le code de la touche générée lorsqu'on redimensionne le terminal
            break;
    }

    endwin();

    return 0;
}