#ifndef DISPLAY_H
#define DISPLAY_H
#include "header.h"

#define YELLOW_COLOR "\033[33m"
#define RESET_COLOR "\033[0m"

void display_directory(char *home_dir);
void printextra(char *input, char *home_dir, int timetaken);

#endif