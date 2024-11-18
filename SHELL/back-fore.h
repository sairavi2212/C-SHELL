#ifndef BACK_FORE
#define BACK_FORE
#include "header.h"
#include "input.h"
int getcommand(int pid);

void sigchld_handler(int num);

void store_background(char *command, int rc);

char *get_foreground(char *command);

void remove_process(int pid);

#endif
