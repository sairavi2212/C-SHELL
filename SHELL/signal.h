#ifndef SIGNAL_H
#define SIGNAL_H
#include "header.h"
#include "input.h"

void ping(char *command);
void handle_ctrl_d();
void handle_sigint(int sig);
void handle_sigtstp(int sig);
char *getproc_withpid(int pid);
void bring_to_foreground(int pid);
void run_in_background(int pid);

#endif
