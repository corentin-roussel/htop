//
// Created by corentin on 30/01/24.
//

#ifndef HTOP_DISPLAY_H
#define HTOP_DISPLAY_H
#include "process.h"
#include <ncurses.h>

void progressBar(float percentage, int placeX, int placeY);
void ncursing();
#endif //HTOP_DISPLAY_H
