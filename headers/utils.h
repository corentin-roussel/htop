//
// Created by corentin on 30/01/24.
//

#ifndef HTOP_UTILS_H
#define HTOP_UTILS_H


#include <stdlib.h>
#include <stdio.h>

char **split(char *src);
int word_counter(const char *src);
int blank(char src);
int my_atoi(char *str);


#endif //HTOP_UTILS_H
